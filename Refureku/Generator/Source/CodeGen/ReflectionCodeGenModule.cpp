#include "RefurekuGenerator/CodeGen/ReflectionCodeGenModule.h"

#include "Kodgen/InfoStructures/StructClassInfo.h"

//#include "RefurekuGenerator/CodeGen/GeneratedEntityCodeTemplate.h"	//generateNativePropertiesCode

using namespace rfk;

std::hash<std::string> ReflectionCodeGenModule::_stringHasher;

ReflectionCodeGenModule::ReflectionCodeGenModule() noexcept
{
	addPropertyCodeGen(_customInstantiatorProperty);
	addPropertyCodeGen(_propertySettingsProperty);
}

ReflectionCodeGenModule::ReflectionCodeGenModule(ReflectionCodeGenModule const&) noexcept:
	ReflectionCodeGenModule()
{
}

ReflectionCodeGenModule* ReflectionCodeGenModule::clone() const noexcept
{
	return new ReflectionCodeGenModule(*this);
}

kodgen::ETraversalBehaviour	ReflectionCodeGenModule::generateHeaderFileHeaderCode(kodgen::EntityInfo const* entity, kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	if (entity == nullptr)
	{
		//Called once at the very beginning of the generation with nullptr entity
		forwardDeclareHeaderTypes(env, inout_result);
	}

	return kodgen::ETraversalBehaviour::Recurse; 
}

kodgen::ETraversalBehaviour	ReflectionCodeGenModule::generateClassFooterCode(kodgen::EntityInfo const* entity, kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	assert(entity != nullptr); //Class footer code should always depend on a non-nullptr entity

	switch (entity->entityType)
	{
		case kodgen::EEntityType::Struct:
			[[fallthrough]];
		case kodgen::EEntityType::Class:
			declareStaticGetArchetypeMethod(reinterpret_cast<kodgen::StructClassInfo const&>(*entity), env, inout_result);
			declareGetArchetypeMethodIfInheritFromObject(reinterpret_cast<kodgen::StructClassInfo const&>(*entity), env, inout_result);

			break;
	}

	return kodgen::ETraversalBehaviour::Recurse;
}

kodgen::ETraversalBehaviour ReflectionCodeGenModule::generateSourceFileHeaderCode(kodgen::EntityInfo const* entity, kodgen::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	if (entity != nullptr)
	{
		switch (entity->entityType)
		{
			case kodgen::EEntityType::Struct:
				[[fallthrough]];
			case kodgen::EEntityType::Class:
				defineStaticGetArchetypeMethod(reinterpret_cast<kodgen::StructClassInfo const&>(*entity), env, inout_result);
				defineGetArchetypeMethodIfInheritFromObject(reinterpret_cast<kodgen::StructClassInfo const&>(*entity), env, inout_result);

				break;
		}
	}
	else
	{
		//Called once at the very beginning of the generation with nullptr entity
		includeRefurekuHeaders(env, inout_result);
	}

	return kodgen::ETraversalBehaviour::Recurse;
}

void ReflectionCodeGenModule::forwardDeclareHeaderTypes(kodgen::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	inout_result +=	"namespace rfk"
					" { "
					"class Struct; "
					"using Class = Struct; "
					"}" + env.getSeparator();
}

void ReflectionCodeGenModule::includeRefurekuHeaders(kodgen::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	//TODO: Can improve this by only including used header files depending on the parsing result for this file.
	inout_result += "#include <Refureku/TypeInfo/Archetypes/Class.h>" + env.getSeparator();

	inout_result += env.getSeparator();

	//inout_result += "#include <Refureku/Misc/DisableWarningMacros.h>\n"
	//				"#include <Refureku/TypeInfo/Namespaces/Namespace.h>\n"
	//				"#include <Refureku/TypeInfo/Namespaces/NamespaceFragment.h>\n"
	//				"#include <Refureku/TypeInfo/Namespaces/NamespaceFragmentRegisterer.h>\n"
	//				"#include <Refureku/TypeInfo/Archetypes/Class.h>\n"
	//				"#include <Refureku/TypeInfo/Archetypes/Enum.h>\n"
	//				"#include <Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h>\n"
	//				"#include <Refureku/TypeInfo/Entity/DefaultEntityRegisterer.h>\n";
}

void ReflectionCodeGenModule::declareStaticGetArchetypeMethod(kodgen::StructClassInfo const& structClass, kodgen::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	inout_result += "static ";
	inout_result += (structClass.entityType == kodgen::EEntityType::Struct) ? "rfk::Struct" : "rfk::Class";
	inout_result += " const& staticGetArchetype() noexcept;" + env.getSeparator();
}

void ReflectionCodeGenModule::declareGetArchetypeMethodIfInheritFromObject(kodgen::StructClassInfo const& structClass, kodgen::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (env.getFileParsingResult()->structClassTree.isBaseOf("rfk::Object", structClass.getFullName()))
	{
		inout_result += "virtual ";
		inout_result += (structClass.entityType == kodgen::EEntityType::Struct) ? "rfk::Struct" : "rfk::Class";
		inout_result += " const& getArchetype() const noexcept;" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::defineStaticGetArchetypeMethod(kodgen::StructClassInfo const& structClass, kodgen::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	std::string returnType = (structClass.entityType == kodgen::EEntityType::Struct) ? "rfk::Struct" : "rfk::Class";

	inout_result += returnType + " const& " + structClass.type.getCanonicalName() + "::staticGetArchetype() noexcept {" + env.getSeparator();
	inout_result += "static bool initialized = false;" + env.getSeparator();
	inout_result += "static " + std::move(returnType) + " type(\"" + structClass.name + "\", " +
											getEntityId(structClass) + ", "
											"sizeof(" + structClass.name + "), " +
											((structClass.entityType == kodgen::EEntityType::Class) ? "true" : "false") +
											");" + env.getSeparator();;

	inout_result += "return type; }" + env.getSeparator() + env.getSeparator();
}

void ReflectionCodeGenModule::defineGetArchetypeMethodIfInheritFromObject(kodgen::StructClassInfo const& structClass, kodgen::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (env.getFileParsingResult()->structClassTree.isBaseOf("rfk::Object", structClass.getFullName()))
	{
		std::string returnType = (structClass.entityType == kodgen::EEntityType::Struct) ? "rfk::Struct" : "rfk::Class";

		inout_result += std::move(returnType) + " const& " + structClass.type.getCanonicalName() + "::getArchetype() const noexcept { return " + structClass.name + "::staticGetArchetype(); }" + env.getSeparator() + env.getSeparator();
	}
}

//std::string const				ReflectionCodeGenModule::_nativePropsMacroName	= std::string(_internalPrefix) + "NativeProperties_GENERATED";
//std::hash<std::string> const	ReflectionCodeGenModule::_stringHasher;

//void FileGenerationUnit::postGenerateFile(kodgen::FileParsingResult const& /* parsingResult */) noexcept
//{
//	_generatedNamespaces.clear();
//	_generatedClasses.clear();
//	_generatedEnums.clear();
//	_generatedVariables.clear();
//	_generatedFunctions.clear();
//
//	_entitiesUsingNativeProperties.clear();
//}
//
//std::string FileGenerationUnit::getEndFileMacroName() const noexcept
//{
//	return "File_GENERATED";
//}
//
//void FileGenerationUnit::writeHeader(kodgen::GeneratedFile& file, kodgen::FileParsingResult const& parsingResult) const noexcept
//{
//	//Always call base class
//	kodgen::FileGenerationUnit::writeHeader(file, parsingResult);
//
//	file.writeLines("#include <Refureku/Misc/DisableWarningMacros.h>",
//					"#include <Refureku/TypeInfo/Namespaces/Namespace.h>",
//					"#include <Refureku/TypeInfo/Namespaces/NamespaceFragment.h>",
//					"#include <Refureku/TypeInfo/Namespaces/NamespaceFragmentRegisterer.h>",
//					"#include <Refureku/TypeInfo/Archetypes/Class.h>",
//					"#include <Refureku/TypeInfo/Archetypes/Enum.h>",
//					"#include <Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h>",
//					"#include <Refureku/TypeInfo/Entity/DefaultEntityRegisterer.h>",
//					"\n");
//}
//
//void FileGenerationUnit::writeFooter(kodgen::GeneratedFile& file, kodgen::FileParsingResult const& parsingResult) const noexcept
//{
//	//Always call base class
//	kodgen::FileGenerationUnit::writeFooter(file, parsingResult);
//
//	file.undefMacro(_nativePropsMacroName);
//	generateNativePropertiesCode(file, parsingResult);
//
//	file.undefMacro(getEndFileMacroName());
//	generateEndFileMacro(file);
//}
//
//bool FileGenerationUnit::writeEntityToFile(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo& entityInfo, kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& out_genResult) noexcept
//{
//	if (kodgen::FileGenerationUnit::writeEntityToFile(generatedFile, entityInfo, parsingResult, out_genResult))
//	{
//		saveEntitiesUsingNativeProperties(entityInfo);
//
//		return true;
//	}
//
//	return false;
//}
//
//bool FileGenerationUnit::writeNamespaceToFile(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo& namespaceInfo, kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& genResult) noexcept
//{
//	if (kodgen::FileGenerationUnit::writeNamespaceToFile(generatedFile, namespaceInfo, parsingResult, genResult))
//	{
//		_generatedNamespaces.push_back(reinterpret_cast<kodgen::NamespaceInfo const*>(&namespaceInfo));
//
//		return true;
//	}
//
//	return false;
//}
//
//bool FileGenerationUnit::writeStructOrClassToFile(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo& structClassInfo, kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& genResult) noexcept
//{
//	if (kodgen::FileGenerationUnit::writeStructOrClassToFile(generatedFile, structClassInfo, parsingResult, genResult))
//	{
//		_generatedClasses.push_back(reinterpret_cast<kodgen::StructClassInfo const*>(&structClassInfo));
//
//		return true;
//	}
//
//	return false;
//}
//
//bool FileGenerationUnit::writeEnumToFile(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo& enumInfo, kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& genResult) noexcept
//{
//	if (kodgen::FileGenerationUnit::writeEnumToFile(generatedFile, enumInfo, parsingResult, genResult))
//	{
//		_generatedEnums.push_back(reinterpret_cast<kodgen::EnumInfo const*>(&enumInfo));
//
//		return true;
//	}
//
//	return false;
//}
//
//bool FileGenerationUnit::writeVariableToFile(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo& variableInfo, kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& genResult) noexcept
//{
//	if (kodgen::FileGenerationUnit::writeVariableToFile(generatedFile, variableInfo, parsingResult, genResult))
//	{
//		_generatedVariables.push_back(reinterpret_cast<kodgen::VariableInfo const*>(&variableInfo));
//
//		return true;
//	}
//
//	return false;
//}
//
//bool FileGenerationUnit::writeFunctionToFile(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo& functionInfo, kodgen::FileParsingResult const& parsingResult, kodgen::FileGenerationResult& genResult) noexcept
//{
//	if (kodgen::FileGenerationUnit::writeFunctionToFile(generatedFile, functionInfo, parsingResult, genResult))
//	{
//		_generatedFunctions.push_back(reinterpret_cast<kodgen::FunctionInfo const*>(&functionInfo));
//
//		return true;
//	}
//
//	return false;
//}
//
//void FileGenerationUnit::generateEndFileMacro(kodgen::GeneratedFile& file) const noexcept
//{
//	file.writeLine("#define " + getEndFileMacroName() + "\t\\");
//
//	//Enum first because structs/classes and namespaces can have nested (and then reference to) enums
//	for (kodgen::EnumInfo const* enumInfo : _generatedEnums)
//	{
//		file.writeLine("	" + std::string(_internalPrefix) + std::to_string(_stringHasher(enumInfo->id)) + "u_GENERATED\t\\");
//	}
//
//	//Gen variables
//	for (kodgen::VariableInfo const* varInfo : _generatedVariables)
//	{
//		file.writeLine("	" + std::string(_internalPrefix) + std::to_string(_stringHasher(varInfo->id)) + "u_GENERATED\t\\");
//	}
//
//	//Gen functions
//	for (kodgen::FunctionInfo const* funcInfo : _generatedFunctions)
//	{
//		file.writeLine("	" + std::string(_internalPrefix) + std::to_string(_stringHasher(funcInfo->id)) + "u_GENERATED\t\\");
//	}
//
//	//Structs/Classes before namespaces because namespaces can have nested (and then reference to) structs/classes
//	for (kodgen::StructClassInfo const* classInfo : _generatedClasses)
//	{
//		file.writeLine("	" + std::string(_internalPrefix) + std::to_string(_stringHasher(classInfo->id)) + "u_GetTypeDefinition\t\\");
//	}
//
//	for (kodgen::NamespaceInfo const* namespaceInfo : _generatedNamespaces)
//	{
//		file.writeLine("	" + std::string(_internalPrefix) + std::to_string(_stringHasher(namespaceInfo->id)) + "u_GENERATED\t\\");
//	}
//
//	//Native properties footer code
//	file.writeLine("	" + _nativePropsMacroName + "\t\\");
//
//	//New line to avoid "warning: backslash-newline at end of file"
//	file.writeLine("\n");
//}
//
//void FileGenerationUnit::saveEntitiesUsingNativeProperties(kodgen::EntityInfo const& entityInfo) noexcept
//{
//	for (kodgen::SimpleProperty const& prop : entityInfo.properties.simpleProperties)
//	{
//		if (prop.boundPropertyRule != nullptr)
//		{
//			_entitiesUsingNativeProperties.push_back(&entityInfo);
//			return;
//		}
//	}
//
//	for (kodgen::ComplexProperty const& prop : entityInfo.properties.complexProperties)
//	{
//		if (prop.boundPropertyRule != nullptr)
//		{
//			_entitiesUsingNativeProperties.push_back(&entityInfo);
//			return;
//		}
//	}
//}
//
//void FileGenerationUnit::generateNativePropertiesCode(kodgen::GeneratedFile& file, kodgen::FileParsingResult const& /* parsingResult */) const noexcept
//{
//	std::string generatedCode;
//
//	//Generate native properties header code
//	PropertyCodeGenFileHeaderData headerData;
//	
//	for (kodgen::EntityInfo const* entityInfo : _entitiesUsingNativeProperties)
//	{
//		assert(entityInfo != nullptr);
//
//		generatedCode += GeneratedEntityCodeTemplate::generateNativePropertiesCode(*entityInfo, &headerData);
//	}
//
//	file.writeLine(generatedCode);
//
//	//Generate native properties footer code inside a macro
//	PropertyCodeGenFileFooterData footerData;
//
//	generatedCode.clear();
//	for (kodgen::EntityInfo const* entityInfo : _entitiesUsingNativeProperties)
//	{
//		assert(entityInfo != nullptr);
//
//		generatedCode += GeneratedEntityCodeTemplate::generateNativePropertiesCode(*entityInfo, &footerData);
//	}
//
//	file.writeMacro(std::string(_nativePropsMacroName), std::move(generatedCode));
//}