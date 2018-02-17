#include <vld.h>
#include "nanovg.h"
#include <stdio.h>
#include <stdlib.h>

static int my_renderCreate(void* uptr)
{
	printf("%s\n", __FUNCTION__);
	return 1;
}
static int my_renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
	int tid = rand();
	printf("%s: %d, %d, %d, %d, %d\n", __FUNCTION__, tid, type, w, h, imageFlags);
	return tid;
}
static int my_renderDeleteTexture(void* uptr, int image)
{
	printf("%s: %d\n", __FUNCTION__, image);
	return 1;
}
static int my_renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
	printf("%s: %d, %d, %d, %d, %d\n", __FUNCTION__, image, x, y, w, h);
	return 1;
}
static int my_renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
	printf("%s: %d\n", __FUNCTION__, image);
	return 1;
}
static void my_renderViewport(void* uptr, int width, int height, float devicePixelRatio)
{
	printf("%s: %d, %d, %g\n", __FUNCTION__, width, height, devicePixelRatio);
}
static void my_renderCancel(void* uptr)
{
	printf("%s\n", __FUNCTION__);
}
static void my_renderFlush(void* uptr)
{
	printf("%s\n", __FUNCTION__);
}

static void PrintVertex(int i, const NVGvertex &vert)
{
	printf("    %4d:%10g,%10g,%4g,%4g\n",
		i,
		vert.x, vert.y,
		vert.u, vert.v);
}
static void PrintPath(const NVGpath &path)
{
	printf("  %d, %d, %d, %d, %d, %d\n",
		path.first,
		path.count,
		path.closed,
		path.nbevel,
		path.winding,
		path.convex);

	printf("  fill:\n");
	for (int i = 0; i < path.nfill; ++i)
		PrintVertex(i, path.fill[i]);

	printf("  stroke:\n");
	for (int i = 0; i < path.nstroke; ++i)
		PrintVertex(i, path.stroke[i]);
}
static void my_renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe, const float* bounds, const NVGpath* paths, int npaths)
{
	printf("%s: [%g, %g], %g, %g, %d, [%g, %g, %g, %g], %g\n", __FUNCTION__,
		paint->extent[0], paint->extent[1],
		paint->radius,
		paint->feather,
		paint->image,
		bounds[0], bounds[1], bounds[2], bounds[3],
		fringe);

	for (int i = 0; i < npaths; ++i)
		PrintPath(paths[i]);
}
static void my_renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe, float strokeWidth, const NVGpath* paths, int npaths)
{
	printf("%s: [%g, %g], %g, %g, %d, %g\n", __FUNCTION__,
		paint->extent[0], paint->extent[1],
		paint->radius,
		paint->feather,
		paint->image,
		fringe);

	for (int i = 0; i < npaths; ++i)
		PrintPath(paths[i]);
}
static void my_renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, const NVGvertex* verts, int nverts)
{
	printf("%s: %d\n", __FUNCTION__, nverts);
}
static void my_renderDelete(void* uptr)
{
	printf("%s\n", __FUNCTION__);
}

//////////////////////////////////////////////////////////////////////////
int main()
{
	NVGparams params{};
	params.renderCreate = my_renderCreate;
	params.renderCreateTexture = my_renderCreateTexture;
	params.renderDeleteTexture = my_renderDeleteTexture;
	params.renderUpdateTexture = my_renderUpdateTexture;
	params.renderGetTextureSize = my_renderGetTextureSize;
	params.renderViewport = my_renderViewport;
	params.renderCancel = my_renderCancel;
	params.renderFlush = my_renderFlush;
	params.renderFill = my_renderFill;
	params.renderStroke = my_renderStroke;
	params.renderTriangles = my_renderTriangles;
	params.renderDelete = my_renderDelete;
	params.userPtr = nullptr;
	params.edgeAntiAlias = 1;

	NVGcontext *ctx = nvgCreateInternal(&params);

	nvgBeginFrame(ctx, 1280, 720, 1);
	{
		nvgBeginPath(ctx);
		nvgCircle(ctx, 10, 10, 15);
		nvgFillColor(ctx, nvgRGBA(220, 160, 0, 200));
		nvgFill(ctx);

		nvgStrokeColor(ctx, nvgRGBA(220, 160, 0, 255));
		nvgStrokeWidth(ctx, 3.0f);
		nvgStroke(ctx);
	}
	nvgEndFrame(ctx);

	nvgDeleteInternal(ctx);

	return 0;
}
