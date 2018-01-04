#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <list>
#include "mu_pathutil.h"
#include "mu_generator.h"


mu_generator::mu_generator(mu_project_ptr pProject) :m_pProject(pProject)
{

}


mu_generator::~mu_generator()
{
}

void mu_generator::generate()
{
	if (!m_pProject)
		return;
	for (int i = 0; i < m_pProject->get_source_count(); ++i)
	{
		mu_source_ptr pSource = m_pProject->get_source(i);
		FILETIME srcWiteTime;
		mu_pathutil::get_file_write_time(pSource->get_file_path(), &srcWiteTime);
		std::ofstream file;
		std::string strFileName;
		std::stringstream strFunc;
		bool initial = false;
		char stringDataName[MAX_PATH] = "";
		LPSTR file_name = NULL;
		for (int j = 0; j < pSource->get_class_count(); ++j)
		{
			mu_class_ptr pClass = pSource->get_class(j);


			if (pClass->get_signal_counts() <= 0
				&& pClass->get_slot_counts() <= 0)
			{
				continue;
			}
			if (!initial)
			{
				char strIncludePath[MAX_PATH] = "";
				std::string strProjectPath = m_pProject->get_project_path();
				strProjectPath += "\\mumu_generate";
				if (!PathFileExists(strProjectPath.data()))
				{
					CreateDirectory(strProjectPath.data(), NULL);
				}
				PathRelativePathTo(strIncludePath, strProjectPath.data(), FILE_ATTRIBUTE_DIRECTORY, pSource->get_file_path().data(), 0);
				strFileName = strProjectPath + "\\" + "mu_" + pSource->get_file_name() + ".cpp";
				FILETIME dstWiteTime;
				if (mu_pathutil::get_file_write_time(strFileName.data(), &dstWiteTime))
				{
					if (CompareFileTime(&srcWiteTime, &dstWiteTime) == 0)
					{
						break;
					}
				}
				strFunc << "#include \"" << strIncludePath << "\"\n#include <string>\n";
				m_pProject->add_generate_file(strFileName);
				std::string stdStringData = pSource->get_string_data();

				memcpy(stringDataName, pSource->get_file_path().data(), pSource->get_file_path().length());
				file_name = PathFindFileName(stringDataName);
				PathRemoveExtension(file_name);
				strFunc << "\nstruct mu_meta_stringdata_" << file_name << "_t\n" << "{\n" << "\tstd::string data["
					<< pSource->get_string_data_count() << "];\n" << "};\n";

				strFunc << "\nstatic const mu_meta_stringdata_" << file_name << "_t mu_meta_stringdata_" << file_name << " =\n"
					<< "{\n" << stdStringData << "\n};\n";
				initial = true;
			}


			std::stringstream strMetaData;
			strMetaData << "\nstatic const unsigned int mu_meta_data_" << pClass->get_name() << "[] =\n"
				<< "{\n"
				<< "\t" << pClass->get_slot_counts() + pClass->get_signal_counts() << ",//method counts\n"
				<< "\t" << pClass->get_signal_counts() << ",//signal counts\n"
				<< "\t//signal:name argCounts arg1 ...";
			std::stringstream strStaticMetaCall;
			strStaticMetaCall << "\nvoid " << pClass->get_name()
				<< "::mu_static_metacall(mu_object *_o, mu_metaobject::Call _c, int _id, void **_a)\n"
				<< "{\n"
				<< "\tif (_c == mu_metaobject::InvokeMetaMethod)\n"
				<< "\t{\n"
				<< "\t\t" << pClass->get_name() << " *_t = static_cast<" << pClass->get_name() << " *>(_o);\n"
				<< "\t\tswitch (_id)\n"
				<< "\t\t{\n";
			std::stringstream strAsyncMetaCall;
			std::stringstream strSignaFunc;
			std::stringstream strTypes;
			std::stringstream strStaticMetaType;
			strStaticMetaType << "\nmu_type* " << pClass->get_name()
				<< "::mu_static_metatype(int _id, void **_a)\n"
				<< "{\n"
				<< "\tswitch(_id)\n"
				<< "\t{\n";
			for (int s = 0; s < (pClass->get_signal_counts() + pClass->get_slot_counts()); ++s)
			{
				mu_function_ptr pFunc;
				if (s >= pClass->get_signal_counts())
				{
					pFunc = pClass->get_slot(s - pClass->get_signal_counts());
				}
				else
				{
					pFunc = pClass->get_sinal(s);
					strSignaFunc << "\n//signal " << pFunc->get_func_index() << "\n" << pFunc->get_return_type() << " "
						<< pClass->get_name() << "::" << pFunc->get_name()
						<< "(" << pFunc->get_format_args() << ")\n{\n"
						<< "\tvoid *_a[] = { 0" << pFunc->get_interpret_format()
						<< " };\n\tmu_metaobject::activate(this, &staticMetaObject, "
						<< pFunc->get_func_index() << ", _a);\n" << "}\n";
				}
				if (s == 0)
				{
					strMetaData << "\n\t" << pFunc->get_meta_data_str();
				}
				else
				{
					strMetaData << ",\n\t" << pFunc->get_meta_data_str();
				}

				strStaticMetaCall << pFunc->get_static_metacall_str();

				std::stringstream strFuncArgs;
				std::stringstream strClassName;
				std::stringstream strInit;
				std::stringstream strInvokeArgs;
				strClassName << "mu_" << pClass->get_name() << "_" << pFunc->get_name();
				for (int a = 0; a < pFunc->get_arg_count(); ++a)
				{
					mu_arg_ptr pArg = pFunc->get_arg(a);
					strClassName << "_" << pArg->get_type_index();
					strFuncArgs << "\t" << pArg->get_type() << " m_" << pArg->get_type_index() << ";\n";
					if (a == 0)
					{
						strInit << ":m_" << pArg->get_type_index() << "(" << pArg->get_static_metacall_str() << ")";
						strInvokeArgs << "_tc->m_" << pArg->get_type_index();
					}
					else
					{
						strInit << ", m_" << pArg->get_type_index() << "(" << pArg->get_static_metacall_str() << ")";
						strInvokeArgs << ", _tc->m_" << pArg->get_type_index();
					}
				}
				strTypes << "\nclass " << strClassName.str() << " :public mu_type\n" << "{\npublic:\n"
					<< strFuncArgs.str() << "\t" << strClassName.str() << "(void** _a)" << strInit.str()
					<< "{}\n" << "};\n";

				strStaticMetaType << "\tcase " << s << ": return new " << strClassName.str() << "(_a);\n";
				strAsyncMetaCall << "\t\tcase " << s << ":\n\t\t{\n"
					<< "\t\t\t" << strClassName.str() << "* _tc = static_cast<" << strClassName.str() << "*>(_a[1]);\n"
					<< "\t\t\t" << pClass->get_name() << "* _t =  static_cast<" << pClass->get_name() << "*>(_o);\n"
					<< "\t\t\t_t->" << pFunc->get_name() << "(" << strInvokeArgs.str() << ");\n"
					<< "\t\t}\n\t\tbreak;\n";
			}
			strStaticMetaType << "\tdefault: return 0;\n\t}\n}\n";
			strMetaData << "\n};\n";

			strStaticMetaCall << "\t\tdefault: break;\n"
				<< "\t\t}\n"
				<< "\t}\n"
				<< "\telse if(_c == mu_metaobject::AsyncInvokeMetaMethod)\n\t{\n"
				<< "\t\tswitch(_id)\n\t\t{\n"
				<< strAsyncMetaCall.str()
				<< "\t\tdefault:\n\t\tbreak;\n\t\t}\n\t}\n"
				<< "}\n";

			std::stringstream strStaticMetaobject;
			strStaticMetaobject << "\nconst mu_metaobject " << pClass->get_name() << "::staticMetaObject = \n"
				<< "{\n" << "\t{\n";
			if (pClass->get_parent_name().empty())
			{
				strStaticMetaobject << "\t\t0,\n";
			}
			else
			{
				strStaticMetaobject << "\t\t&" << pClass->get_parent_name() << "::staticMetaObject,\n";
			}
			strStaticMetaobject << "\t\tmu_meta_stringdata_" << file_name << ".data,\n"
				<< "\t\tmu_meta_data_" << pClass->get_name() << ",\n"
				<< "\t\tmu_static_metacall,\n"
				<< "\t\tmu_static_metatype,\n"
				<< "\t\t0\n"
				<< "\t}\n" << "};\n"
				<< "\nconst mu_metaobject* " << pClass->get_name() << "::metaObject() const\n"
				<< "{\n" << "\treturn &staticMetaObject;\n" << "}\n";

			strFunc << strMetaData.str() << strTypes.str() << strStaticMetaType.str()
				<< strStaticMetaCall.str() << strStaticMetaobject.str() << strSignaFunc.str();

		}
		if (!strFunc.str().empty())
		{
			file.open(strFileName, std::ios::trunc | std::ios::out);
			file << strFunc.str();
			file.close();
			mu_pathutil::set_file_write_time(strFileName.data(), &srcWiteTime);
		}
	}

}
