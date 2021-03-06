#pragma once

#include "config.h"
#include "gvc.h"
#include "gvio.h"

#include "gvplugin.h"

using namespace System;


#include <stdlib.h>
#include <time.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define DEMAND_LOADING 1

extern "C" size_t gv_bitmap_write(GVJ_t * job, const char *s, size_t len);


public ref class GraphResult : public IDisposable
{
public:
		
public:
	System::String^ Text;
	System::Drawing::Bitmap^ Image;
	GraphResult(System::String^ Text, System::Drawing::Bitmap^ Image)
		:Text(Text)
		, Image(Image)
	{

	}
	!GraphResult()
	{
		this->~GraphResult();
	}
	~GraphResult()
	{
		this->Text = nullptr;
		if (this->Image != nullptr) {
			delete this->Image;
			this->Image = nullptr;
		}
	}
};
public ref class GraphProvider
{
public:
	static System::Drawing::Bitmap^ Working;

	static GraphProvider() {
		Working = nullptr;
		lt_preloaded_symbols->address = 0;
		lt_preloaded_symbols->name = 0;
	}
public:
	const int BYTES_PER_PIXEL = 4;

public:
	static GraphResult ^ Generate(System::String^ Graph) {
		return Generate(Graph, "dot");
	}
	static GraphResult ^ Generate(System::String^ Graph, System::String^ Layout) {
		return Generate(Graph, Layout, "jpg");
	}
	static GraphResult^ Generate(System::String^ Graph, System::String^ Layout, System::String^ Type) {
		if (Graph == nullptr) throw gcnew ArgumentNullException("Graph");
		if (Type == nullptr) throw gcnew ArgumentNullException("Type");

		char* GraphData = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(Graph).ToPointer();
		char* LayoutData = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(Layout).ToPointer();
		char* TypeData = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(Type).ToPointer();

		GraphResult^ Result = Generate(GraphData, LayoutData, TypeData);

		System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(TypeData));
		System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(LayoutData));
		System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(GraphData));

		return Result;
	}
protected:
	static GraphResult ^ Generate(const char* Graph, char* Layout, char* Type) {
		int r = 0;

		System::Drawing::Bitmap^ Image = nullptr;
		System::String^ Text = nullptr;

		GVC_t* gvContext = gvContextPlugins(lt_preloaded_symbols, DEMAND_LOADING, gv_bitmap_write);
		if (gvContext != 0) {

			char* args[] = { Layout };

			gvParseArgs(gvContext, 1, args);

			Agraph_t* g = agmemread(Graph);

			if (g != 0) {
				gvInitGraph(gvContext, g, "<internal>", 0);

				r = gvLayoutJobs(gvContext, g);

				char* Result = 0;

				unsigned int ResultLength = 0;

				r = gvRenderData(gvContext, g, Type, &Result, &ResultLength);

				if (Result != 0) {
					Text = System::Runtime::InteropServices::Marshal::PtrToStringAnsi(IntPtr(Result));
					free(Result);
				}

				Image = Working;

				r = gvFreeLayout(gvContext, g);

				agclose(g);
			}
			gvFreeContext(gvContext);
		}

		return gcnew GraphResult(Text, Image);
	}
public:
	static int Process(array<System::String^>^ Args) {

		int ret = 0;
		char** argv = new char*[Args->Length];
		for (int i = 0; i < Args->Length; i++) {

			argv[i] = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(Args[i]).ToPointer();
		}

		ret = Process(Args->Length, argv);

		for (int i = 0; i < Args->Length; i++) {

			System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(argv[i]));
		}


		delete[] argv;

		return ret;
	}
protected:

	static int Process(int argc, char **argv)
	{
		graph_t *prev = NULL;
		int r, rc = 0;
		GVC_t *Gvc = 0;
		graph_t *G = 0;
		Gvc = gvContextPlugins(lt_preloaded_symbols, DEMAND_LOADING, FALSE);

		gvParseArgs(Gvc, argc, argv);


		if ((G = gvPluginsGraph(Gvc))) {
			gvLayoutJobs(Gvc, G);  /* take layout engine from command line */
			gvRenderJobs(Gvc, G);
		}
		else {
			while ((G = gvNextInputGraph(Gvc))) {
				if (prev) {
					gvFreeLayout(Gvc, prev);
					agclose(prev);
				}
				gvLayoutJobs(Gvc, G);  /* take layout engine from command line */
				gvRenderJobs(Gvc, G);
				gvFinalize(Gvc);
				r = agreseterrors();
				rc = MAX(rc, r);
				prev = G;
			}
		}
		r = gvFreeContext(Gvc);
		return (MAX(rc, r));
	}

};

