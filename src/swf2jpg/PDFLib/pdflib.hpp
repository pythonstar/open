/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2002 PDFlib GmbH and Thomas Merz. All rights reserved. |
 +---------------------------------------------------------------------------+
 |    This software is NOT in the public domain.  It can be used under two   |
 |    substantially different licensing terms:                               |
 |                                                                           |
 |    The commercial license is available for a fee, and allows you to       |
 |    - ship a commercial product based on PDFlib                            |
 |    - implement commercial Web services with PDFlib                        |
 |    - distribute (free or commercial) software when the source code is     |
 |      not made available                                                   |
 |    Details can be found in the file PDFlib-license.pdf.                   |
 |                                                                           |
 |    The "Aladdin Free Public License" doesn't require any license fee,     |
 |    and allows you to                                                      |
 |    - develop and distribute PDFlib-based software for which the complete  |
 |      source code is made available                                        |
 |    - redistribute PDFlib non-commercially under certain conditions        |
 |    - redistribute PDFlib on digital media for a fee if the complete       |
 |      contents of the media are freely redistributable                     |
 |    Details can be found in the file aladdin-license.pdf.                  |
 |                                                                           |
 |    These conditions extend to ports to other programming languages.       |
 |    PDFlib is distributed with no warranty of any kind. Commercial users,  |
 |    however, will receive warranty and support statements in writing.      |
 *---------------------------------------------------------------------------*/

// $Id: pdflib.hpp,v 1.14.2.7 2002/01/07 18:26:29 tm Exp $
//
// C++ wrapper for PDFlib
//
//

#ifndef PDFLIB_HPP
#define PDFLIB_HPP

#ifndef PDF_THROWS_CPP_EXCEPTIONS
#define PDF_THROWS_CPP_EXCEPTIONS
#endif

#include <string>

	using namespace std;

// We use PDF as a C++ class name, therefore hide the actual C struct
// name for PDFlib usage with C++.

#define PDF PDF_c
#include "pdflib.h"
#ifdef	PDF_THROWS_CPP_EXCEPTIONS
#include "except.h"
#endif
#undef	PDF

// #define PDF_THROWS_CPP_EXCEPTIONS

#ifdef	PDF_THROWS_CPP_EXCEPTIONS
#define PDF_THROWS(x)		throw (x)
#define PDF_THROWS_NOTHING	throw ()
#else
#define PDFlib PDF
#define PDF_THROWS(x)
#define PDF_THROWS_NOTHING
#endif


// The C++ class wrapper for PDFlib

class PDFlib {
public:
#ifdef	PDF_THROWS_CPP_EXCEPTIONS
    class Exception
    {
    public:
	Exception(string msg, void *opaque);
	string get_message();
	const void *get_opaque();

    private:
	string m_msg;
	const void *m_opaque;
    }; // Exception

    class MemoryException : public Exception
    {
    public:
	MemoryException(string msg, void *opaque);
    };
    class IOException : public Exception
    {
    public:
	IOException(string msg, void *opaque);
    };
    class RuntimeException : public Exception
    {
    public:
	RuntimeException(string msg, void *opaque);
    };
    class IndexException : public Exception
    {
    public:
	IndexException(string msg, void *opaque);
    };
    class TypeException : public Exception
    {
    public:
	TypeException(string msg, void *opaque);
    };
    class DivisionByZeroException : public Exception
    {
    public:
	DivisionByZeroException(string msg, void *opaque);
    };
    class OverflowException : public Exception
    {
    public:
	OverflowException(string msg, void *opaque);
    };
    class SyntaxException : public Exception
    {
    public:
	SyntaxException(string msg, void *opaque);
    };
    class ValueException : public Exception
    {
    public:
	ValueException(string msg, void *opaque);
    };
    class SystemException : public Exception
    {
    public:
	SystemException(string msg, void *opaque);
    };
    class NonfatalException : public Exception
    {
    public:
	NonfatalException(string msg, void *opaque);
    };
    class UnknownException : public Exception
    {
    public:
	UnknownException(string msg, void *opaque);
    };

    PDFlib(allocproc_t allocproc = NULL,
	reallocproc_t reallocproc = NULL,
	freeproc_t freeproc = NULL,
	void *opaque = NULL) PDF_THROWS(Exception);

#else // PDF_THROWS_CPP_EXCEPTIONS

    PDFlib(errorproc_t errorproc = NULL,
	allocproc_t allocproc = NULL,
	reallocproc_t reallocproc = NULL,
	freeproc_t freeproc = NULL,
	void *opaque = NULL);

#endif // PDF_THROWS_CPP_EXCEPTIONS

    ~PDFlib() PDF_THROWS_NOTHING;

    void * get_opaque() PDF_THROWS(Exception);
    static int get_majorversion() PDF_THROWS_NOTHING;
    static int get_minorversion() PDF_THROWS_NOTHING;

    // Overloaded generic open and close methods
    int  open(string filename) PDF_THROWS(Exception);
    int  open(FILE *fp) PDF_THROWS(Exception);
    void open(writeproc_t writeproc) PDF_THROWS(Exception);

    void close() PDF_THROWS(Exception);

    const char *get_buffer(long *size) PDF_THROWS(Exception);

    void begin_page(float width, float height) PDF_THROWS(Exception);
    void end_page() PDF_THROWS(Exception);
    void set_parameter(string key, string value) PDF_THROWS(Exception);
    string get_parameter(string key, float mod) PDF_THROWS(Exception);
    float get_value(string key, float mod) PDF_THROWS(Exception);
    void set_value(string key, float value) PDF_THROWS(Exception);
    void show(string text) PDF_THROWS(Exception);
    void show_xy(string text, float x, float y) PDF_THROWS(Exception);
    void continue_text(string text) PDF_THROWS(Exception);
    int show_boxed(string text, float left, float top,
	float width, float height, string hmode, string reserved)
	PDF_THROWS(Exception);
    void set_text_pos(float x, float y) PDF_THROWS(Exception);

    float stringwidth(string text, int font, float size)
	PDF_THROWS(Exception);

    int  findfont(string fontname, string encoding, int embed)
	PDF_THROWS(Exception);
    void setfont(int font, float fontsize) PDF_THROWS(Exception);
    void save() PDF_THROWS(Exception);
    void restore() PDF_THROWS(Exception);
    void translate(float tx, float ty) PDF_THROWS(Exception);
    void scale(float sx, float sy) PDF_THROWS(Exception);
    void rotate(float phi) PDF_THROWS(Exception);
    void skew(float alpha, float beta) PDF_THROWS(Exception);
    void concat(float a, float b, float c, float d, float e, float f)
	PDF_THROWS(Exception);
    void setdash(float b, float w) PDF_THROWS(Exception);
    void setpolydash(float *darray, int length) PDF_THROWS(Exception);
    void setflat(float flat) PDF_THROWS(Exception);
    void setlinejoin(int join) PDF_THROWS(Exception);
    void setlinecap(int cap) PDF_THROWS(Exception);
    void setmiterlimit(float miter) PDF_THROWS(Exception);
    void setlinewidth(float width) PDF_THROWS(Exception);
    void moveto(float x, float y) PDF_THROWS(Exception);
    void lineto(float x, float y) PDF_THROWS(Exception);
    void curveto(float x1, float y1, float x2, float y2, float x3, float y3)
	PDF_THROWS(Exception);
    void circle(float x, float y, float r) PDF_THROWS(Exception);
    void arc(float x, float y, float r, float alpha1, float alpha2)
	PDF_THROWS(Exception);
    void arcn(float x, float y, float r, float alpha1, float alpha2)
	PDF_THROWS(Exception);
    void rect(float x, float y, float width, float height)
	PDF_THROWS(Exception);
    void closepath() PDF_THROWS(Exception);
    void stroke() PDF_THROWS(Exception);
    void closepath_stroke() PDF_THROWS(Exception);
    void fill() PDF_THROWS(Exception);
    void fill_stroke() PDF_THROWS(Exception);
    void closepath_fill_stroke() PDF_THROWS(Exception);
    void clip() PDF_THROWS(Exception);
    void endpath() PDF_THROWS(Exception);
    void setgray_fill(float g) PDF_THROWS(Exception);
    void setgray_stroke(float g) PDF_THROWS(Exception);
    void setgray(float g) PDF_THROWS(Exception);
    void setrgbcolor_fill(float red, float green, float blue)
	PDF_THROWS(Exception);
    void setrgbcolor_stroke(float red, float green, float blue)
	PDF_THROWS(Exception);
    void setrgbcolor(float red, float green, float blue) PDF_THROWS(Exception);
    int begin_template(float width, float height) PDF_THROWS(Exception);
    void end_template() PDF_THROWS(Exception);
    void place_image(int image, float x, float y, float scale)
	PDF_THROWS(Exception);
    int open_image(string type, string source, const char *data,
	long len, int width, int height, int components, int bpc,
	string params) PDF_THROWS(Exception);
    int open_image_file(string type, string filename,
    	string stringparam, int intparam) PDF_THROWS(Exception);
    void close_image(int image) PDF_THROWS(Exception);
    int open_CCITT(string filename, int width, int height,
    	bool BitReverse, int K, bool BlackIs1) PDF_THROWS(Exception);
    int add_bookmark(string text, int parent, bool open)
	PDF_THROWS(Exception);
    void set_info(string key, string value) PDF_THROWS(Exception);
    void attach_file(float llx, float lly, float urx, float ury,
    	string filename, string description, string author,
	string mimetype, string icon) PDF_THROWS(Exception);
    void add_note(float llx, float lly, float urx, float ury,
    	string contents, string title, string icon, bool open)
	PDF_THROWS(Exception);
    void add_pdflink(float llx, float lly, float urx, float ury,
    	string filename, int page, string dest) PDF_THROWS(Exception);
    void add_launchlink(float llx, float lly, float urx, float ury,
    	string filename) PDF_THROWS(Exception);
    void add_locallink(float llx, float lly, float urx, float ury,
    	int page, string dest) PDF_THROWS(Exception);
    void add_weblink(float llx, float lly, float urx, float ury, string url)
	PDF_THROWS(Exception);

    void set_border_style(string style, float width) PDF_THROWS(Exception);
    void set_border_color(float red, float green, float blue)
	PDF_THROWS(Exception);
    void set_border_dash(float d1, float d2) PDF_THROWS(Exception);

    int open_pdi(string filename, string stringparam, int intparam)
	PDF_THROWS(Exception);
    void close_pdi(int doc) PDF_THROWS(Exception);
    int open_pdi_page(int doc, int page, string label) PDF_THROWS(Exception);
    void place_pdi_page(int page, float x, float y, float sx, float sy)
	PDF_THROWS(Exception);
    void close_pdi_page(int page) PDF_THROWS(Exception);
    string get_pdi_parameter(string key, int doc, int page, int index,
	int *len) PDF_THROWS(Exception);
    float get_pdi_value(string key, int doc, int page, int index)
	PDF_THROWS(Exception);

    int begin_pattern(float width, float height, float xstep, float ystep,
	int painttype) PDF_THROWS(Exception);
    void end_pattern() PDF_THROWS(Exception);
    int makespotcolor(string spotname, int len) PDF_THROWS(Exception);
    void setcolor(string type, string colorspace,
	float c1, float c2, float c3, float c4) PDF_THROWS(Exception);
    void add_thumbnail(int image) PDF_THROWS(Exception);
    void initgraphics() PDF_THROWS(Exception);
    void setmatrix(float a, float b, float c, float d, float e, float f)
	PDF_THROWS(Exception);

private:
    PDF_c *p;
#ifdef	PDF_THROWS_CPP_EXCEPTIONS
    void throw_exception() PDF_THROWS(Exception);
    pdf_err_info err_info;
#endif
};

#endif	// PDFLIB_HPP

