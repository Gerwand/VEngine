#pragma once
#include <errno.h>

#define VE_CTXBUSY		EBUSY		/* Context is already occupied.						*/
#define VE_EVAL			EINVAL		/* Invalid argument.								*/
#define VE_EMODE		EACCES		/* In current mode, this operation is not permited.	*/
#define VE_EIO			EIO			/* IO operation fail.								*/
#define VE_FAULT		EFAULT		/* Critical fault, returned on assertion fail.		*/
#define VE_NOHANDLE		ENODEV		/* Failed to get handle from gl functions.			*/
#define VE_NORESOURCES	ENODATA		/* Did not get any resources/ null pointer			*/
#define VE_EMEM			ENOMEM