#ifndef BITMAP_DISPLAY_DRIVER
#define BITMAP_DISPLAY_DRIVER
#include <ndspy.h>
//Register driver:

/*PtDspyDriverFunctionTable table;
memset(&table, 0, sizeof(table));
table.Version = k_PtDriverCurrentVersion;
table.pOpen = &MayaDspyImageOpen;
table.pQuery = &MayaDspyImageQuery;
table.pWrite = &MayaDspyImageData;
table.pClose = &MayaDspyImageClose;
DspyRegisterDriverTable("maya_render_view", &table);
*/



PtDspyError BmpDspyImageQuery(PtDspyImageHandle image,
	PtDspyQueryType type, 
	int size, 
	void *data);

PtDspyError BmpDspyImageOpen(PtDspyImageHandle * image,
	const char *drivername,
	const char *filename,
	int width,
	int height,
	int paramcount,
	const UserParameter *parameters,
	int formatcount,
	PtDspyDevFormat *format,
	PtFlagStuff *flags);

PtDspyError BmpDspyImageData(PtDspyImageHandle image,
	int xmin,
	int xmax_plus_one,
	int ymin,
	int ymax_plus_one,
	int entrysize,
	const unsigned char*data);


PtDspyError BmpDspyImageClose(PtDspyImageHandle image);

#endif