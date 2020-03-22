#include "GeneratedCodeTemplate.h"

#include <cassert>

using namespace refureku;

void GeneratedCodeTemplate::undefMacros(kodgen::GeneratedFile& generatedFile, std::string const& generatedMacroName) const noexcept
{
	//TODO
}

void GeneratedCodeTemplate::generateCode(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo const& entityInfo) const noexcept
{
	switch (entityInfo.entityType)
	{
		case kodgen::EntityInfo::EType::Class:
			generateClassCode(generatedFile, static_cast<kodgen::StructClassInfo const&>(entityInfo));
			break;

		case kodgen::EntityInfo::EType::Struct:
			generateStructCode(generatedFile, static_cast<kodgen::StructClassInfo const&>(entityInfo));
			break;

		case kodgen::EntityInfo::EType::Enum:
			generateEnumCode(generatedFile, static_cast<kodgen::EnumInfo const&>(entityInfo));
			break;

		default:
			assert(false);	//Should never reach this point
	}
}

void GeneratedCodeTemplate::generateClassCode(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& classInfo) const noexcept
{
	std::string mainMacroName	 = "RFRK" + classInfo.name + "_GENERATED";

	std::string getTypeMacroName = generateGetTypeMacro(generatedFile, classInfo);

	generatedFile.writeMacro(std::move(mainMacroName),
								std::move(getTypeMacroName),
								"private:");
}

void GeneratedCodeTemplate::generateStructCode(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& structInfo) const noexcept
{
	//std::string mainMacroName	 = "RFRK" + structInfo.name + "_GENERATED";

	//std::string getTypeMacroName = generateGetTypeMacro(generatedFile, structInfo);

	//generatedFile.writeMacro(std::move(mainMacroName),
	//						 std::move(getTypeMacroName),
	//						 "public:");
}

void GeneratedCodeTemplate::generateEnumCode(kodgen::GeneratedFile& generatedFile, kodgen::EnumInfo const& enumInfo) const noexcept
{

}

std::string GeneratedCodeTemplate::generateGetTypeMacro(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& info) const noexcept
{
	std::string getTypeMacroName					= "RFRK" + info.name + "_GetTypeMacro";
	std::string generatedMethodsMetadataMacroName	= generateMethodsMetadataMacro(generatedFile, info);

	generatedFile.writeMacro(std::string(getTypeMacroName),
								"public:",
								"	static refureku::Type const& staticGetType() noexcept",
								"	{",
								"		static bool				initialized = false;",
								"		static refureku::Type	type;",
								"	",
								"		if (!initialized)",
								"		{",
								"			" + std::move(generatedMethodsMetadataMacroName),
								"			initialized = true;",
								"		}",
								"	",
								"		return type;",
								"	}",
								"	",
								"	refureku::Type const& getType() const noexcept",
								"	{",
								"		return " + info.name + "::staticGetType();",
								"	}"
							 );

	return getTypeMacroName;
}

std::string GeneratedCodeTemplate::generateMethodsMetadataMacro(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& info) const noexcept
{
	std::string macroName = "RFRK" + info.name + "_GenerateMethodsMetadata";

	generatedFile.writeLine("#define " + macroName + "\t\\");

	for (auto& [accessSpecifier, methods] : info.methods)
	{
		for (kodgen::MethodInfo const& method : methods)
		{
			if (method.qualifiers.isStatic)
			{
				//Fill static method table lookup
				generatedFile.writeLine("	type.staticMethodLookupTable.emplace(\"" + method.name + "\", refureku::StaticMethod(new refureku::NonMemberFunction<" + method.prototype + ">(& " + info.name + "::" + method.name + ")));\t\\");
			}
			else
			{
				//Remove const from method prototype if any
				std::string methodProto(method.prototype);
				if (methodProto.back() == 't')	//If proto ends with a t, it should be the cons't'
					methodProto.resize(methodProto.size() - 5u);	//5 is the size of "const"

				//Fill method table lookup
				generatedFile.writeLine("	type.methodLookupTable.emplace(\"" + method.name + "\", refureku::Method(new refureku::MemberFunction<" + info.name + ", " + std::move(methodProto) + ">(& " + info.name + "::" + method.name + ")));\t\\");
			}
		}
	}

	generatedFile.writeLine("");

	return macroName;
}