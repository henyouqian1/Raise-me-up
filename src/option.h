#ifndef __OPTION_H__
#define __OPTION_H__

void optionLoad();
void optionSave();

bool getOption(const char* key, int& outValue);
void setOption(const char* key, int value);

bool getOptionStr(const char* key, std::string& outValue);
void setOptionStr(const char* key, const char* value);

bool getOptionData(const char* key, const char** ppData, int& size);
void setOptionData(const char* key, const char* pData, int size);

#endif //__OPTION_H__