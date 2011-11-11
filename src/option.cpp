#include "stdafx.h"
#include "option.h"

namespace{
	std::map<std::string, int> _intMap;
	std::map<std::string, std::string> _strMap;
}

void optionLoad(){
	_intMap.clear();
	_strMap.clear();
	sqlite3_stmt* pStmt = NULL;
	{
		std::stringstream ss;
		ss << "select key, value from option;";
		int r = sqlite3_prepare_v2(g_pSaveDb, ss.str().c_str(), -1, &pStmt, NULL);
		lwassert(r == SQLITE_OK);
		while ( 1 ){
			r = sqlite3_step(pStmt);
			if ( r == SQLITE_ROW ){
				const char* key = (const char*)sqlite3_column_text(pStmt, 0);
				int v = sqlite3_column_int(pStmt, 1);
				_intMap[key] = v;
			}else if ( r == SQLITE_DONE ){
				break;
			}else{
				//lwassert(0);
				break;
			}
		}
		sqlite3_finalize(pStmt);
	}
	{
		std::stringstream ss;
		ss << "select key, value from optionStr;";
		int r = sqlite3_prepare_v2(g_pSaveDb, ss.str().c_str(), -1, &pStmt, NULL);
		lwassert(r == SQLITE_OK);
		while ( 1 ){
			r = sqlite3_step(pStmt);
			if ( r == SQLITE_ROW ){
				const char* key = (const char*)sqlite3_column_text(pStmt, 0);
				const char* v = (const char*)sqlite3_column_text(pStmt, 1);
				_strMap[key] = v;
			}else if ( r == SQLITE_DONE ){
				break;
			}else{
				//lwassert(0);
				break;
			}
		}
		sqlite3_finalize(pStmt);
	}
}

void optionSave(){
	std::stringstream ss;
	ss << "BEGIN TRANSACTION;";

	{
		std::map<std::string, int>::iterator it = _intMap.begin();
		std::map<std::string, int>::iterator itEnd = _intMap.end();
		for ( ; it != itEnd; ++it ){
			ss << "insert into option(key, value) select \"" << it->first <<  "\", 1 where not exists(select 1 from option where key=\""<< it->first << "\");";
			ss << "update option set value=" << it->second << " where key=\""<< it->first <<"\";";
		}
	}
	{
		std::map<std::string, std::string>::iterator it = _strMap.begin();
		std::map<std::string, std::string>::iterator itEnd = _strMap.end();
		for ( ; it != itEnd; ++it ){
			ss << "insert into optionStr(key, value) select \"" << it->first <<  "\", 1 where not exists(select 1 from optionStr where key=\""<< it->first << "\");";
			ss << "update optionStr set value=\"" << it->second << "\" where key=\""<< it->first <<"\";";
		}
	}
	
	ss << "COMMIT;";
	int r = sqlite3_exec(g_pSaveDb, ss.str().c_str(), NULL, NULL, NULL);
	lwassert(r == SQLITE_OK);
}

bool getOption(const char* key, int& outValue){
	std::map<std::string, int>::iterator it = _intMap.find(key);
	if ( it == _intMap.end() ){
		return false;
	}
	outValue = it->second;
	return true;
}

void setOption(const char* key, int value){
	_intMap[key] = value;
}

bool getOptionStr(const char* key, std::string& outValue){
	std::map<std::string, std::string>::iterator it = _strMap.find(key);
	if ( it == _strMap.end() ){
		return false;
	}
	outValue = it->second;
	return true;
}

void setOptionStr(const char* key, const char* value){
	_strMap[key] = value;
}

bool getOptionData(const char* key, const char** ppData, int& size){
	bool rt = false;
	sqlite3_stmt* pStmt = NULL;
	{
		std::stringstream ss;
		ss << "select data from optionData where key=\"" << key << "\";";
		int r = sqlite3_prepare_v2(g_pSaveDb, ss.str().c_str(), -1, &pStmt, NULL);
		lwassert(r == SQLITE_OK);
		r = sqlite3_step(pStmt);
		if ( r == SQLITE_ROW ){
			size = sqlite3_column_bytes(pStmt, 0);
			*ppData = new char[size];
			char* p = (char*)sqlite3_column_blob(pStmt, 0);
			memcpy((void*)*ppData, p, size);
			rt = true;
		}
		sqlite3_finalize(pStmt);
	}
	return rt;
}

void setOptionData(const char* key, const char* pData, int size){
	sqlite3_stmt* pStmt = NULL;
	std::stringstream ss;
	//ss << "insert into optionData(key, data) select \"" << key <<  "\", 1 where not exists(select 1 from option where key=\""<< key << "\");";
	//int r = sqlite3_prepare_v2(g_pSaveDb, ss.str().c_str(), -1, &pStmt, NULL);
	//if (r == SQLITE_OK){
	//	r = sqlite3_step(pStmt);
	//	if ( r != SQLITE_DONE ){
	//		lwerror("r != SQLITE_DONE");
	//	}
	//}
	//sqlite3_finalize(pStmt);
	{
		std::stringstream ss;
		ss << "update optionData set data = ?1 where key = \""<< key << "\";";
		int r = sqlite3_prepare_v2(g_pSaveDb, ss.str().c_str(), -1, &pStmt, NULL);
		if (r == SQLITE_OK){
			sqlite3_bind_blob(pStmt, 1, pData, size, NULL);
			r = sqlite3_step(pStmt);
			if ( r != SQLITE_DONE ){
				lwerror("r != SQLITE_DONE");
			}
		}
		sqlite3_finalize(pStmt);
	}
}