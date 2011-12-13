#include "stdafx.h"
#include "httpClient.h"

namespace {
    lw::HTTPClient* g_pClient = NULL;
}

lw::HTTPClient* getHttpClient(){
    return g_pClient;
}

void initHttpClient(){
    lwassert(g_pClient == NULL);
    g_pClient = new lw::HTTPClient(L"www.2b22b.com");
    lwassert(g_pClient);
}

void destroyHttpClient(){
    lwassert(g_pClient);
    delete g_pClient;
    g_pClient = NULL;
}

void httpClientMain(){
    assert(g_pClient);
    g_pClient->main();
}