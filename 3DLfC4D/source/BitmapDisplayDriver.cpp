#include "BitmapDisplayDriver.h"
#include "c4d.h"

const void* GetParameter(
	const char *name,
	unsigned n,
	const UserParameter parms[])
{
	for (unsigned i = 0; i<n; i++)
	{
		if (0 == strcmp(name, parms[i].name))
		{
			return parms[i].value;
		}
	}
	return 0x0;
}

PtDspyError BmpDspyImageQuery(PtDspyImageHandle image,
	PtDspyQueryType type,
	int size,
	void *data){
	GePrint("DspyImageQuery");
	return PkDspyErrorNone;
}

PtDspyError BmpDspyImageOpen(PtDspyImageHandle * image,
	const char *drivername,
	const char *filename,
	int width,
	int height,
	int paramcount,
	const UserParameter *parameters,
	int formatcount,
	PtDspyDevFormat *format,
	PtFlagStuff *flags){
	
	BaseBitmap** bmp_ptr = (BaseBitmap**)GetParameter("BaseBitmap", paramcount, parameters);

	if (!bmp_ptr){
		GePrint("Didn't get bitmap");
		return PkDspyErrorBadParams;
	}
	else{
		GePrint("Got bitmap");
	}

	BaseBitmap* bmp = *bmp_ptr;
	bmp->Init(width, height);
	image = (void**)bmp;

	return PkDspyErrorNone;
}

PtDspyError BmpDspyImageData(PtDspyImageHandle image,
	int xmin,
	int xmax_plus_one,
	int ymin,
	int ymax_plus_one,
	int entrysize,
	const unsigned char*data){
	GePrint("Bucket");
	BaseBitmap* bmp = (BaseBitmap*)image;

	for (int x = xmin; x < xmax_plus_one; x++){
		for (int y = ymin; y < ymax_plus_one; y++){
			bmp->SetPixel(x, y, 255, 255, 255);
		}
	}
	//ShowBitmap(bmp);
	return PkDspyErrorNone;
}

PtDspyError BmpDspyImageClose(PtDspyImageHandle image){
	GePrint("DspyImageClose");
	return PkDspyErrorNone;
}