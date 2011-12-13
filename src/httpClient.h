#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

lw::HTTPClient* getHttpClient();
void initHttpClient();
void destroyHttpClient();
void httpClientMain();

#endif //__HTTP_CLIENT_H__