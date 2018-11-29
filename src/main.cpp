#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <direct.h>
using namespace std;

#include "tinyxml/tinyxml.h"
#include "Str.h"


std::vector<std::string> split(std::string str, std::string pattern);



bool make_dsw_file(const char* project_name)
{
	const char* dsw_content = 
		"Microsoft Developer Studio Workspace File, Format Version 6.00\r\n"
		"# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\r\n"
		"\r\n"
		"###############################################################################\r\n"
		"\r\n"
		"Project: \"%s\"=\".\\%s.dsp\" - Package Owner=<4>\r\n"
		"\r\n"
		"Package=<5>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"Package=<4>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"###############################################################################\r\n"
		"\r\n"
		"Global:\r\n"
		"\r\n"
		"Package=<5>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"Package=<3>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"###############################################################################\r\n"
		"\r\n"
		"";
	
	char dsw[1024] = {0};
	string name(project_name);
	name += ".dsw";
	FILE* fp_dsw = fopen(name.c_str(),"wb");
	if(fp_dsw == NULL){
		cout<<"错误:无法打开 "<<name<<" 用于写!"<<endl;
		return false;
	}

	int len = _snprintf(dsw,sizeof(dsw),dsw_content,project_name,project_name);
	if(fwrite(dsw,1,len,fp_dsw) == len){
		cout<<"已创建 "<<name<<" 工作空间文件!"<<endl;
		fclose(fp_dsw);
		return true;
	}else{
		cout<<"错误:文件写入错误!"<<endl;
		fclose(fp_dsw);
		remove(name.c_str());
		return false;
	}
}

bool make_dsp_file(const char* project_name,vector<string>& groups,string& define,string& includepath)
{
	const char* dsp_content = 
		"# Microsoft Developer Studio Project File - Name=\"%s\" - Package Owner=<4>\r\n"
		"# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n"
		"# ** DO NOT EDIT **\r\n"
		"\r\n"
		"# TARGTYPE \"Win32 (x86) Console Application\" 0x0103\r\n"
		"\r\n"
		"CFG=%s - Win32 Debug\r\n"
		"!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n"
		"!MESSAGE use the Export Makefile command and run\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE NMAKE /f \"%s.mak\".\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE You can specify a configuration when running NMAKE\r\n"
		"!MESSAGE by defining the macro CFG on the command line. For example:\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE Possible choices for configuration are:\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) Console Application\")\r\n"
		"!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Console Application\")\r\n"
		"!MESSAGE \r\n" /*在这之前有7个项目名*/
		"\r\n"
		"# Begin Project\r\n"
		"# PROP AllowPerConfigDependencies 0\r\n"
		"# PROP Scc_ProjName \"\"\r\n"
		"# PROP Scc_LocalPath \"\"\r\n"
		"CPP=cl.exe\r\n"
		"RSC=rc.exe\r\n"
		"\r\n"
		"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\r\n" //项目名
		"\r\n"
		"# PROP BASE Use_MFC 0\r\n"
		"# PROP BASE Use_Debug_Libraries 0\r\n"
		"# PROP BASE Output_Dir \"Release\"\r\n"
		"# PROP BASE Intermediate_Dir \"Release\"\r\n"
		"# PROP BASE Target_Dir \"\"\r\n"
		"# PROP Use_MFC 0\r\n"
		"# PROP Use_Debug_Libraries 0\r\n"
		"# PROP Output_Dir \"Release\"\r\n"
		"# PROP Intermediate_Dir \"Release\"\r\n"
		"# PROP Target_Dir \"\"\r\n"
		"# ADD BASE CPP /nologo /W3 /GX /O2 %s /YX /FD /c\r\n" //宏定义:/D \"WIN32\" /D \"NDEBUG\"
		"# ADD CPP /nologo /W3 /GX /O2 %s %s /YX /FD /c\r\n" //目录+宏定义
		"# ADD BASE RSC /l 0x804 /d \"NDEBUG\"\r\n"
		"# ADD RSC /l 0x804 /d \"NDEBUG\"\r\n"
		"BSC32=bscmake.exe\r\n"
		"# ADD BASE BSC32 /nologo\r\n"
		"# ADD BSC32 /nologo\r\n"
		"LINK32=link.exe\r\n"
		"# ADD BASE LINK32 user32.lib /nologo /subsystem:console /machine:I386\r\n"
		"# ADD LINK32 user32.lib /nologo /subsystem:console /machine:I386\r\n"
		"\r\n"
		"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n"     //项目名
		"\r\n"
		"# PROP BASE Use_MFC 0\r\n"
		"# PROP BASE Use_Debug_Libraries 1\r\n"
		"# PROP BASE Output_Dir \"Debug\"\r\n"
		"# PROP BASE Intermediate_Dir \"Debug\"\r\n"
		"# PROP BASE Target_Dir \"\"\r\n"
		"# PROP Use_MFC 0\r\n"
		"# PROP Use_Debug_Libraries 1\r\n"
		"# PROP Output_Dir \"Debug\"\r\n"
		"# PROP Intermediate_Dir \"Debug\"\r\n"
		"# PROP Target_Dir \"\"\r\n"
		"# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od %s /YX /FD /GZ  /c\r\n" //宏定义
		"# ADD CPP /nologo /W3 /Gm /GX /ZI /Od %s %s /YX /FD /GZ  /c\r\n" //目录+宏定义
		"# ADD BASE RSC /l 0x804 /d \"_DEBUG\"\r\n"
		"# ADD RSC /l 0x804 /d \"_DEBUG\"\r\n"
		"BSC32=bscmake.exe\r\n"
		"# ADD BASE BSC32 /nologo\r\n"
		"# ADD BSC32 /nologo\r\n"
		"LINK32=link.exe\r\n"
		"# ADD BASE LINK32 user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
		"# ADD LINK32 user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
		"\r\n"
		"!ENDIF \r\n" /*前面只有2个项目名*/
		"\r\n"
		"# Begin Target\r\n"
		"\r\n"
		"# Name \"%s - Win32 Release\"\r\n"    //这里有两个
		"# Name \"%s - Win32 Debug\"\r\n"
		;

	char dsp[100*1024] = {0};
	string name(project_name);
	name += ".dsp";
	FILE* fp_dsp = fopen(name.c_str(),"wb");
	if(fp_dsp == NULL){
		cout<<"错误:无法打开 "<<name<<" 用于写!"<<endl;
		return false;
	}

	// 定义是逗号分隔的
	define = define + ",__CC_ARM";	//添加__CC_ARM宏定义以正确包含stdint.h
	char* zDefine = new char[define.length()+2];
	memset(zDefine,0,define.length()+2);
	strcpy(zDefine,define.c_str());
	for(size_t iii=0; iii<define.length()+2; iii++){
		if(zDefine[iii]==','){
			zDefine[iii] = '\0';
		}
	}
	string strDefine("");
	for(const char* pp=zDefine;*pp;){
		strDefine += "/D \"";
		strDefine += pp;
		strDefine += "\" ";

		while(*pp) pp++;
		pp++;
	}

	// includepath是分号分隔的
	char* zInclude = new char[includepath.length()+2];
	memset(zInclude,0,includepath.length()+2);
	strcpy(zInclude,includepath.c_str());
	for(size_t jjj=0; jjj<includepath.length()+2; jjj++){
		if(zInclude[jjj]==';'){
			zInclude[jjj] = '\0';
		}
	}
	
	string strInclude("");
	for(const char* qq=zInclude;*qq;){
		strInclude += "/I \"";
		strInclude += qq;
		strInclude += "\" ";
		
		while(*qq) qq++;
		qq++;
	}

	int len=0;
	len += _snprintf(dsp,sizeof(dsp)-len,dsp_content,
		project_name,project_name,project_name,project_name,project_name,project_name,project_name,
		
		project_name,
		strDefine.c_str(),
		strInclude.c_str(),strDefine.c_str(),
		
		project_name,
		strDefine.c_str(),
		strInclude.c_str(),strDefine.c_str(),

		project_name,
		project_name
	);

	delete[] zDefine;
	delete[] zInclude;

	int len_write = 0;
	for(auto & group : groups){
		 len_write = _snprintf(dsp+len,sizeof(dsp)-len,"%s",group.c_str());
		 len += len_write;
	}

	len += _snprintf(dsp+len,sizeof(dsp)-len,"%s","# End Target\r\n# End Project\r\n");

	if(fwrite(dsp,1,len,fp_dsp) == len){
		cout<<"已创建 "<<name<<" 项目文件!"<<endl;
		fclose(fp_dsp);
	}else{
		cout<<"错误:文件写入错误!"<<endl;
		fclose(fp_dsp);
		remove(name.c_str());
		return false;
	}
	return true;
}

bool get_uv_info(const char* uvproj,vector<string>& groups,string& define,string& includepath)
{
	const char* ret_str=NULL;
	bool bXmlUtf8 = true;

	TiXmlDocument doc(uvproj);
	if(!doc.LoadFile()){
		cout<<"错误:TiXmlDocument.LoadFile()"<<endl;
		return false;
	}

	bXmlUtf8 = stricmp(doc.FirstChild()->ToDeclaration()->Encoding(),"UTF-8")==0;
	
	TiXmlNode* nodeTarget = doc.FirstChild("Project")->FirstChild("Targets")->FirstChild("Target");
	TiXmlNode* nodeVariousControls = nodeTarget->FirstChild("TargetOption")->FirstChild("TargetArmAds")->FirstChild("Cads")->FirstChild("VariousControls");
	//取得宏定义
	TiXmlElement* cDefine = nodeVariousControls->FirstChild("Define")->ToElement();
	ret_str = cDefine->GetText();
	if(ret_str){
		if(bXmlUtf8){
			define = AStr(ret_str,true).toAnsi();
		}else{
			define = ret_str;
		}
	}
	//取得包含路径
	TiXmlElement* cIncludePath = nodeVariousControls->FirstChild("IncludePath")->ToElement();
	ret_str = cIncludePath->GetText();
	auto inc_vector = split(ret_str, ";");
	stringstream str_to_write;
	str_to_write << "..\\;";
	for ( auto& incs:inc_vector)	//截取每个include项目
	{
		if (incs.length()<2)
			continue;

		int idx;
		while ((idx = incs.find('/')) != string::npos)	//替换所有的"/"文件符号
			incs.replace(idx, 1,"\\");

		if (incs.c_str()[0] == '.')
		{
			if (incs.c_str()[1] == '.')
				str_to_write << "..\\" << incs;
			else
				str_to_write << "." << incs;
		}
		else if (incs.c_str()[0] == '\\')
			str_to_write << ".." << incs;
		else
			str_to_write << "..\\" << incs;
		str_to_write << ";";
	}
	if(str_to_write.str().c_str()){
		if(bXmlUtf8){
			includepath = AStr(str_to_write.str().c_str(),true).toAnsi();
		}else{
			includepath = str_to_write.str();
		}
	}

	//cout<<cDefine->GetText()<<endl;
	//cout<<cIncludePath->GetText()<<endl;

	TiXmlNode* nodeGroups = nodeTarget->FirstChild("Groups");
	//Goups包含源代码分组信息,遍历
	for(TiXmlNode* group=nodeGroups->FirstChild(); group!=NULL; group=group->NextSibling()){
		stringstream strGroup("");
		//取得分组名称
		TiXmlElement* eleGroupName = group->FirstChild("GroupName")->ToElement();
		
		strGroup<<"# Begin Group \""<<AStr(eleGroupName->GetText(),bXmlUtf8).toAnsi()<<"\"\r\n\r\n";
		strGroup<<"# PROP Default_Filter \"\"\r\n";
		//cout<<eleGroupName->GetText()<<endl;

		//Files包含所有的文件列表
		TiXmlNode* nodeFiles = group->FirstChild("Files");
		if(nodeFiles){
			for(TiXmlNode* file=nodeFiles->FirstChild("File"); file!=NULL; file=file->NextSibling()){
				//文件名
				TiXmlElement* eleFileName = file->FirstChild()->ToElement();
				//文件路径
				TiXmlElement* eleFilePath = file->FirstChild()->NextSibling()->NextSibling()->ToElement();
				//cout<<eleFileName->GetText()<<","<<eleFilePath->GetText()<<endl;
				
				string str(AStr(eleFilePath->GetText(), bXmlUtf8).toAnsi());

				if (str[str.length() - 1] != 'h' || str[str.length() - 2] != '.')		//排除*.h文件
				{
					string str_to_write;
					int idx;
					while ((idx = str.find('/')) != string::npos)	//替换所有的"/"符号
						str.replace(idx, 1, "\\");

					if (str.c_str()[0] == '.')	// ./或 .\开始
					{
						if (str.c_str()[1] == '\\')
							str_to_write = string(".") + str;
						else if (str.c_str()[1] == '.')
							str_to_write = string("..\\") + str;
						else
							cout << "存在错误路径: " << str << endl;
					}
					else if (str.c_str()[0] == '\\')		//斜杠开始
						str_to_write = string("..") + str;
					else//单独文件夹名开始
						str_to_write = string("..\\") + str;

					if (str_to_write[str_to_write.length() - 1] != 'h' || str_to_write[str_to_write.length() - 2] != '.')
						strGroup << "# Begin Source File\r\n\r\nSOURCE=\"" << str_to_write << "\"\r\n" << "# End Source File\r\n";
				}
			}
		}
		strGroup<<"# End Group\r\n\r\n";
		groups.emplace_back(strGroup.str());
	}
	return true;
}



//字符串分割函数
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

void dequote(char* str)
{
	char* di = str;
	char* si = str;
	if(*si!=' ' && *si!='\t' && *si!='\"') return;

	while(*si && *si!='\"'){
		si++;
	}

	if(!*si) return;
	else si++;

	while(*si && *si!='\"'){
		*di++ = *si++;
	}
	*di = '\0';
}

void dereturn(char* str)
{
	char* p = str+strlen(str)-1;
	if(*p=='\n') *p = '\0';
}

void setdir(char* location)
{
	char tmp;
	char* p = location+strlen(location)-1;
	while(*p!='\\' && p>=location) p--;
	if(p>=location){
		tmp = *p;
		*p = '\0';
		string proj_path(location);
		proj_path += "\\VSProj";
		mkdir(proj_path.c_str());
		SetCurrentDirectory(proj_path.c_str());
		*p = tmp;
	}
}

bool is_file_present(const char* fn)
{
	DWORD dwAttributes = GetFileAttributes(fn);
	return dwAttributes!=INVALID_FILE_ATTRIBUTES &&
		!(dwAttributes&FILE_ATTRIBUTE_DIRECTORY);
}

void about(void)
{
	const char* help = 
		"   欢迎使用 Keil uVision stm32 项目文件转 Visual Studio 项目小工具\n"
		"如果你厌倦了Keil那糟糕的代码编辑器, 试试回到VS的怀抱吧~ VC6也远比Keil好用哦~\n"
		"\n"
		"使用方法:\n"
		"  项目名称:你的项目的名称\n"
		"  项目路径:Keil uVision stm32 .uvproj 文件绝对路径\n"
		"  系统路径:Keil uVision MDK 提供的头文件所在的绝对路径(Keil/ARM/RV31)\n"
		"\n"
		"关于程序:\n"
		"  作者:女孩不哭 编写时间:2013-10-29 联系:anhbk@qq.com\n"
		"  源码下载:http://www.cnblogs.com/nbsofer/p/keil2vs.html\n\n"
	;
	printf(help);
}

int main(int argc,char** argv)
{
	vector<string> groups;
	string define;
	string includepath;

	char location[260]={0};
	char compiler[260] = { 0 };

	about();
	setlocale(LC_ALL, "chs");

	cout << R"(修改项目原码下载：https://github.com/barryxiaoyi/keil2vs.git)"<< "\r\n" << endl;


	for(;;){
		cout << "项目路径:" << endl;
		fgets(location,sizeof(location),stdin);
		if(*location=='\n'){
			cout<<"错误:路径不合法!"<<endl;
			continue;
		}else{
			dequote(location);
			dereturn(location);
			if(!is_file_present(location)){
				cout << "错误:不存在的 .uvproj 文件\n" << endl;
				continue;
			}
			break;
		}
	}

	string proj_name(location);
	auto name_offset_start = proj_name.find_last_of('\\')+1;
	auto name_offset_end = proj_name.find_last_of('.');
	proj_name = proj_name.substr(name_offset_start, name_offset_end - name_offset_start);

	for(;;){
		cout << "系统路径:" << endl;
		fgets(compiler,sizeof(compiler),stdin);
		if(compiler[0]=='\n'){
			compiler[0] = '\0';
		}
		dequote(compiler);
		dereturn(compiler);
		break;
	}

	string link_dir(compiler);
	if (link_dir.length() == 0)
	{
		link_dir = R"(C:\Keil_v5\ARM\ARMCC\include)";
		cout << "  无有效路径，使用默认：";
	}
	else
		cout << "  路径 :";
	cout << link_dir << endl;

	setdir(location);

	try{
		if(make_dsw_file(proj_name.c_str())){
			if(get_uv_info(location,groups,define,includepath)){
				includepath = link_dir+";" + includepath;
				if(make_dsp_file(proj_name.c_str(),groups,define,includepath)){

				}
			}
		}
	}
	catch(...){
		cout << "创建出错\r\n" << endl;
	}
	system("pause");

	return 0;
}
