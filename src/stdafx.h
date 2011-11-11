// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef WIN32
#   ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#       define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#   endif						

#   include <stdio.h>
#   include <tchar.h>
#endif

#ifdef __APPLE__
#   ifdef __OBJC__
#       import <UIKit/UIKit.h>
#       import <Foundation/Foundation.h>
#   endif
#   ifdef __cplusplus
#       include "lwTinyFrame/lwMinMax.h"
#   endif
#endif

#ifdef __cplusplus
#   include <sstream>
#   include "lwTinyFrame/lwPreInclude.h"
#	include "lwTinyFrame/lwUtil.h"
#   include "lwTinyFrame/lwTask.h"
#   include "lwTinyFrame/lwApp.h"
#   include "lwTinyFrame/lwFileSys.h"
#	include "lwTinyFrame/lwResourceHolder.h"
#	include "lwTinyFrame/lwTextureRes.h"
#	include "lwTinyFrame/lwSprite.h"
#	include "lwTinyFrame/lwFont.h"
#	include "lwTinyFrame/lwUIText.h"
#	include "lwTinyFrame/lwUIButton.h"
#	include "lwTinyFrame/lwUICheckBox.h"
#	include "lwTinyFrame/lwUIImage.h"
#	include "lwTinyFrame/lwUIMenu.h"
#	include "lwTinyFrame/lwTouchEvent.h"
#	include "lwTinyFrame/lwTextField.h"
#	include "lwTinyFrame/lwHTTPClient.h"
#	include "lwTinyFrame/lwSound.h"
#	include "Box2D.h"
#	include "sqlite/sqlite3.h"

#	include "geomDraw.h"
extern sqlite3* g_pDataDb;
extern sqlite3* g_pSaveDb;
#endif

// TODO: reference additional headers your program requires here
