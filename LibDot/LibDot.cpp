#include "stdafx.h"

#include "LibDot.h"


extern "C" lt_symlist_t lt_preloaded_symbols[] = { { 0, 0 } };

size_t gv_bitmap_write(GVJ_t * job, const char *s, size_t len)
{
	if (s != 0 && len > 0) {
		GVJ_s* sjob = (GVJ_s*)job;
		System::IO::UnmanagedMemoryStream^ ums = gcnew System::IO::UnmanagedMemoryStream((unsigned char*)s, len);

		GraphProvider::Working
			= gcnew System::Drawing::Bitmap(ums);

		delete ums;
	}
	return len;
}


