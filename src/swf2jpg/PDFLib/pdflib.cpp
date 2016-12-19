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

// $Id: pdflib.cpp,v 1.13.2.4 2002/01/07 18:26:29 tm Exp $
//
// Implementation of C++ wrapper for PDFlib
//
//

#include "stdafx.h"
#include "pdflib.hpp"

#define CHAR(s)	(s).c_str()

#ifdef	PDF_THROWS_CPP_EXCEPTIONS

PDFlib::Exception::Exception(string msg, void *opaque)
:   m_msg(msg),
    m_opaque(opaque)
{ }

string PDFlib::Exception::get_message()	{ return m_msg;		}
const void *PDFlib::Exception::get_opaque()	{ return m_opaque;	}

PDFlib::MemoryException::MemoryException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::IOException::IOException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::RuntimeException::RuntimeException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::IndexException::IndexException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::TypeException::TypeException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::DivisionByZeroException::DivisionByZeroException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::OverflowException::OverflowException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::SyntaxException::SyntaxException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::ValueException::ValueException(string msge, void *opaque)
:   PDFlib::Exception(msge, opaque)
{ }
PDFlib::SystemException::SystemException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::NonfatalException::NonfatalException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }
PDFlib::UnknownException::UnknownException(string msg, void *opaque)
:   PDFlib::Exception(msg, opaque)
{ }

void PDFlib::throw_exception() PDF_THROWS(PDFlib::Exception)
{
    switch (err_info.type)
    {
	case PDF_MemoryError:
	    throw MemoryException(err_info.msg, err_info.opaque);
	case PDF_IOError:
	    throw IOException(err_info.msg, err_info.opaque);
	case PDF_RuntimeError:
	    throw RuntimeException(err_info.msg, err_info.opaque);
	case PDF_IndexError:
	    throw IndexException(err_info.msg, err_info.opaque);
	case PDF_TypeError:
	    throw TypeException(err_info.msg, err_info.opaque);
	case PDF_DivisionByZero:
	    throw DivisionByZeroException(err_info.msg, err_info.opaque);
	case PDF_OverflowError:
	    throw OverflowException(err_info.msg, err_info.opaque);
	case PDF_SyntaxError:
	    throw SyntaxException(err_info.msg, err_info.opaque);
	case PDF_ValueError:
	    throw ValueException(err_info.msg, err_info.opaque);
	case PDF_SystemError:
	    throw SystemException(err_info.msg, err_info.opaque);
	case PDF_NonfatalError:
	    throw NonfatalException(err_info.msg, err_info.opaque);

	default:
	    throw UnknownException(err_info.msg, err_info.opaque);
    }
}

#define PDF_TRY		if (setjmp(err_info.jbuf) == 0)
#define PDF_CATCH	else throw_exception()

#else

#define PDF_TRY
#define PDF_CATCH	

#endif // PDF_THROWS_CPP_EXCEPTIONS


#ifdef	PDF_THROWS_CPP_EXCEPTIONS

PDFlib::PDFlib(
    allocproc_t allocproc,
    reallocproc_t reallocproc,
    freeproc_t freeproc,
    void *opaque) PDF_THROWS(PDFlib::Exception)
{
    PDF_boot();
    err_info.opaque = opaque;

    PDF_TRY
    {
	p = ::PDF_new2(pdf_cpp_errorhandler,
		    allocproc, reallocproc, freeproc, (void *) &err_info);
    }
    PDF_CATCH;

    PDF_set_parameter(p, "binding", "C++");
}

#else // PDF_THROWS_CPP_EXCEPTIONS

PDFlib::PDFlib(
    errorproc_t errorproc,
    allocproc_t allocproc,
    reallocproc_t reallocproc,
    freeproc_t freeproc,
    void *opaque)
{
    PDF_boot();

    p = ::PDF_new2(errorproc, allocproc, reallocproc, freeproc, opaque);

    PDF_set_parameter(p, "binding", "C++");
}

#endif // PDF_THROWS_CPP_EXCEPTIONS


PDFlib::~PDFlib() PDF_THROWS_NOTHING
{
    PDF_TRY	::PDF_delete(p);
    ::PDF_shutdown();
}

void *
PDFlib::get_opaque() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_get_opaque(p);
    PDF_CATCH;

    return 0;	// compilers love it
}

int
PDFlib::get_majorversion() PDF_THROWS_NOTHING
{
    return ::PDF_get_majorversion();
}

int
PDFlib::get_minorversion() PDF_THROWS_NOTHING
{
    return ::PDF_get_minorversion();
}

int
PDFlib::open(string filename) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_open_file(p, CHAR(filename));
    PDF_CATCH;

    return 0;	// compilers love it
}

int
PDFlib::open(FILE *fp) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_open_fp(p, fp);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::open(writeproc_t writeproc) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_open_mem(p, writeproc);
    PDF_CATCH;
}

void
PDFlib::close() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_close(p);
    PDF_CATCH;
}

const char *
PDFlib::get_buffer(long *size) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_get_buffer(p, size);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::begin_page(float width, float height) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_begin_page(p, width, height);
    PDF_CATCH;
}

void
PDFlib::end_page() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_end_page(p);
    PDF_CATCH;
}

void
PDFlib::set_parameter(string key, string value) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_parameter(p, CHAR(key), CHAR(value));
    PDF_CATCH;
}

string
PDFlib::get_parameter(string key, float mod) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_get_parameter(p, CHAR(key), mod);
    PDF_CATCH;

    return "";	// compilers love it
}

float
PDFlib::get_value(string key, float mod) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_get_value(p, CHAR(key), mod);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::set_value(string key, float value) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_value(p, CHAR(key), value);
    PDF_CATCH;
}

void
PDFlib::show(string text) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_show2(p, text.c_str(), (int) text.size());
    PDF_CATCH;
}

void
PDFlib::show_xy(string text, float x, float y) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_show_xy2(p, text.c_str(), (int) text.size(), x, y);
    PDF_CATCH;
}

void
PDFlib::continue_text(string text) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_continue_text2(p, text.c_str(), (int) text.size());
    PDF_CATCH;
}

int
PDFlib::show_boxed(string text, float left, float top,
    float width, float height, string hmode, string reserved)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	return ::PDF_show_boxed(p, CHAR(text), left, top, width, height,
				    CHAR(hmode), CHAR(reserved));
    }
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::set_text_pos(float x, float y) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_text_pos(p, x, y);
    PDF_CATCH;
}

float
PDFlib::stringwidth(string text, int font, float size)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	return ::PDF_stringwidth2(p, text.c_str(), (int) text.size(),
					font, size);
    }
    PDF_CATCH;

    return 0;	// compilers love it
}

int
PDFlib::findfont(string fontname, string encoding, int embed)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
	return ::PDF_findfont(p, CHAR(fontname), CHAR(encoding), embed);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::setfont(int font, float fontsize) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setfont(p, font, fontsize);
    PDF_CATCH;
}

void
PDFlib::save() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_save(p);
    PDF_CATCH;
}

void
PDFlib::restore() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_restore(p);
    PDF_CATCH;
}

void
PDFlib::translate(float tx, float ty) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_translate(p, tx, ty);
    PDF_CATCH;
}

void
PDFlib::scale(float sx, float sy) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_scale(p, sx, sy);
    PDF_CATCH;
}

void
PDFlib::rotate(float phi) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_rotate(p, phi);
    PDF_CATCH;
}

void
PDFlib::skew(float alpha, float beta) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_skew(p, alpha, beta);
    PDF_CATCH;
}

void
PDFlib::concat(float a, float b, float c, float d, float e, float f)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_concat(p, a, b, c, d, e, f);
    PDF_CATCH;
}

void
PDFlib::setdash(float b, float w) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setdash(p, b, w);
    PDF_CATCH;
}

void
PDFlib::setpolydash(float *darray, int length) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setpolydash(p, darray, length);
    PDF_CATCH;
}

void
PDFlib::setflat(float flat) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setflat(p, flat);
    PDF_CATCH;
}

void
PDFlib::setlinejoin(int join) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setlinejoin(p, join);
    PDF_CATCH;
}

void
PDFlib::setlinecap(int cap) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setlinecap(p, cap);
    PDF_CATCH;
}

void
PDFlib::setmiterlimit(float miter) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setmiterlimit(p, miter);
    PDF_CATCH;
}

void
PDFlib::setlinewidth(float width) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setlinewidth(p, width);
    PDF_CATCH;
}

void
PDFlib::moveto(float x, float y) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_moveto(p, x, y);
    PDF_CATCH;
}

void
PDFlib::lineto(float x, float y) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_lineto(p, x, y);
    PDF_CATCH;
}

void
PDFlib::curveto(float x1, float y1, float x2, float y2, float x3, float y3)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_curveto(p, x1, y1, x2, y2, x3, y3);
    PDF_CATCH;
}

void
PDFlib::circle(float x, float y, float r) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_circle(p, x, y, r);
    PDF_CATCH;
}

void
PDFlib::arc(float x, float y, float r, float alpha1, float alpha2)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_arc(p, x, y, r, alpha1, alpha2);
    PDF_CATCH;
}

void
PDFlib::arcn(float x, float y, float r, float alpha1, float alpha2)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_arcn(p, x, y, r, alpha1, alpha2);
    PDF_CATCH;
}

void
PDFlib::rect(float x, float y, float width, float height)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_rect(p, x, y, width, height);
    PDF_CATCH;
}

void
PDFlib::closepath() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_closepath(p);
    PDF_CATCH;
}

void
PDFlib::stroke() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_stroke(p);
    PDF_CATCH;
}

void
PDFlib::closepath_stroke() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_closepath_stroke(p);
    PDF_CATCH;
}

void
PDFlib::fill() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_fill(p);
    PDF_CATCH;
}

void
PDFlib::fill_stroke() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_fill_stroke(p);
    PDF_CATCH;
}

void
PDFlib::closepath_fill_stroke() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_closepath_fill_stroke(p);
    PDF_CATCH;
}

void
PDFlib::clip() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_clip(p);
    PDF_CATCH;
}

void
PDFlib::endpath() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_endpath(p);
    PDF_CATCH;
}

void
PDFlib::setgray_fill(float g) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setcolor(p, "fill", "gray", g, 0, 0, 0);
    PDF_CATCH;
}

void
PDFlib::setgray_stroke(float g) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setcolor(p, "stroke", "gray", g, 0, 0, 0);
    PDF_CATCH;
}

void
PDFlib::setgray(float g) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setcolor(p, "both", "gray", g, 0, 0, 0);
    PDF_CATCH;
}

void
PDFlib::setrgbcolor_fill(float red, float green, float blue)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setcolor(p, "fill", "rgb", red, green, blue, 0);
    PDF_CATCH;
}

void
PDFlib::setrgbcolor_stroke(float red, float green, float blue)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setcolor(p, "stroke", "rgb", red, green, blue, 0);
    PDF_CATCH;
}

void
PDFlib::setrgbcolor(float red, float green, float blue)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setcolor(p, "both", "rgb", red, green, blue, 0);
    PDF_CATCH;
}

int
PDFlib::begin_template(float width, float height)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_begin_template(p, width, height);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::end_template() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_end_template(p);
    PDF_CATCH;
}

void
PDFlib::place_image(int image, float x, float y, float scale)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_place_image(p, image, x, y, scale);
    PDF_CATCH;
}

int
PDFlib::open_image(string type, string source, const char *data, long len,
    int width, int height, int components, int bpc, string params)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	return ::PDF_open_image(p, CHAR(type), CHAR(source), data, len,
				width, height, components, bpc, CHAR(params));
    }
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::close_image(int image) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_close_image(p, image);
    PDF_CATCH;
}

int
PDFlib::open_image_file(string type, string filename,
    string stringparam, int intparam) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	return ::PDF_open_image_file(p, CHAR(type), CHAR(filename),
					CHAR(stringparam), intparam);
    }
    PDF_CATCH;

    return 0;	// compilers love it
}

int
PDFlib::open_CCITT(string filename, int width, int height, bool BitReverse,
    int K, bool BlackIs1) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	return ::PDF_open_CCITT(p, CHAR(filename), width, height,
				    BitReverse, K, BlackIs1);
    }
    PDF_CATCH;

    return 0;	// compilers love it
}

int
PDFlib::add_bookmark(string text, int parent, bool open)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_add_bookmark(p, CHAR(text), parent, open);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::set_info(string key, string value) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_info(p, CHAR(key), CHAR(value));
    PDF_CATCH;
}

void
PDFlib::attach_file(float llx, float lly, float urx, float ury,
    string filename, string description, string author,
    string mimetype, string icon) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	::PDF_attach_file(p, llx, lly, urx, ury, CHAR(filename),
	    CHAR(description), CHAR(author), CHAR(mimetype), CHAR(icon));
    }
    PDF_CATCH;
}

void
PDFlib::add_note(float llx, float lly, float urx, float ury, string contents,
    string title, string icon, bool open) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_add_note(p, llx, lly, urx, ury, CHAR(contents),
    	CHAR(title), CHAR(icon), open);
    PDF_CATCH;
}

void
PDFlib::add_pdflink(float llx, float lly, float urx, float ury,
    string filename, int page, string dest) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
    {
	::PDF_add_pdflink(p, llx, lly, urx, ury, CHAR(filename),
			    page, CHAR(dest));
    }
    PDF_CATCH;
}

void
PDFlib::add_launchlink(float llx, float lly, float urx, float ury,
    string filename) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_add_launchlink(p, llx, lly, urx, ury, CHAR(filename));
    PDF_CATCH;
}

void
PDFlib::add_locallink(float llx, float lly, float urx, float ury, int page,
    string dest) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_add_locallink(p, llx, lly, urx, ury, page, CHAR(dest));
    PDF_CATCH;
}

void
PDFlib::add_weblink(float llx, float lly, float urx, float ury,
    string url) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_add_weblink(p, llx, lly, urx, ury, CHAR(url));
    PDF_CATCH;
}

void
PDFlib::set_border_style(string style, float width) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_border_style(p, CHAR(style), width);
    PDF_CATCH;
}

void
PDFlib::set_border_color(float red, float green, float blue)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_border_color(p, red, green, blue);
    PDF_CATCH;
}

void
PDFlib::set_border_dash(float d1, float d2) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_set_border_dash(p, d1, d2);
    PDF_CATCH;
}

int
PDFlib::open_pdi(string filename, string stringparam, int intparam)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
	return ::PDF_open_pdi(p, CHAR(filename), CHAR(stringparam), intparam);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::close_pdi(int doc) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_close_pdi(p, doc);
    PDF_CATCH;
}

int
PDFlib::open_pdi_page(int doc, int page, string label)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_open_pdi_page(p, doc, page, CHAR(label));
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::place_pdi_page(int page, float x, float y, float sx, float sy)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_place_pdi_page(p, page, x, y, sx, sy);
    PDF_CATCH;
}

void
PDFlib::close_pdi_page(int page) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_close_pdi_page(p, page);
    PDF_CATCH;
}

string
PDFlib::get_pdi_parameter(string key, int doc, int page, int index, int *len)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
	return ::PDF_get_pdi_parameter(p, CHAR(key), doc, page, index, len);
    PDF_CATCH;

    return "";	// compilers love it
}

float
PDFlib::get_pdi_value(string key, int doc, int page, int index)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_get_pdi_value(p, CHAR(key), doc, page, index);
    PDF_CATCH;

    return 0;	// compilers love it
}

int
PDFlib::begin_pattern(float width, float height, float xstep, float ystep,
    int painttype) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
	return ::PDF_begin_pattern(p, width, height, xstep, ystep, painttype);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::end_pattern() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_end_pattern(p);
    PDF_CATCH;
}

int
PDFlib::makespotcolor(string spotname, int len) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	return ::PDF_makespotcolor(p, CHAR(spotname), len);
    PDF_CATCH;

    return 0;	// compilers love it
}

void
PDFlib::setcolor(string type, string colorspace,
    float c1, float c2, float c3, float c4) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY
	::PDF_setcolor(p, CHAR(type), CHAR(colorspace), c1, c2, c3, c4);
    PDF_CATCH;
}

void
PDFlib::add_thumbnail(int image) PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_add_thumbnail(p, image);
    PDF_CATCH;
}

void
PDFlib::initgraphics() PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_initgraphics(p);
    PDF_CATCH;
}

void
PDFlib::setmatrix( float a, float b, float c, float d, float e, float f)
    PDF_THROWS(PDFlib::Exception)
{
    PDF_TRY	::PDF_setmatrix(p, a, b, c, d, e, f);
    PDF_CATCH;
}

