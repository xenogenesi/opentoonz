

// character_manager.cpp: implementation of the TFont class
// for FreeType 2.
//
//////////////////////////////////////////////////////////////////////

//#include <ft2build.h>
//#include FT_FREETYPE_H


#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>


#include <vector>
#include <iostream>
#include <string>

#include "tpixelgr.h"
#include "tfont.h"
#include "tstroke.h"
//#include "tcurves.h"
#include "traster.h"
#include <vector>
#include <iostream>
#include <string>
//#include <tstring.h>
#include <tmathutil.h>
//#include <tdebugmessage.h>
#include "tvectorimage.h"
using namespace std;



//=============================================================================

struct TFont::Impl {
	bool m_hasKerning;
	int m_hasVertical;
	QFont m_font;
	//XXX:cache QFontMetrics m_metrics; ?

	//  KerningPairs m_kerningPairs;

/*
	ATSUStyle m_style;
	ATSUFontID m_fontId;
	ATSUTextLayout m_layout;
	Fixed m_size;
	int m_ascender;
	int m_descender;
*/

	Impl(const QString &family, const QString &style, int size);
	~Impl();

	//void getChar();
};

//-----------------------------------------------------------------------------

TFont::TFont(const wstring family, const wstring face, int size)
{
	m_pimpl = new Impl(QString::fromStdWString(family),
		QString::fromStdWString(face), size);
}

//-----------------------------------------------------------------------------

TFont::~TFont()
{
	delete m_pimpl;
}

//-----------------------------------------------------------------------------

TFont::Impl::Impl(const QString &family, const QString &style, int size)
{
	m_font = QFont(family, size);
	m_font.setStyleName(style);

}

//-----------------------------------------------------------------------------

TFont::Impl::~Impl()
{
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

TPoint TFont::drawChar(TVectorImageP &image, wchar_t charcode, wchar_t nextCharCode) const
{
	//FIXME
	return TPoint(0, 0);
}

//-----------------------------------------------------------------------------

TPoint TFont::drawChar(TRasterGR8P &outImage, TPoint &unused, wchar_t charcode, wchar_t nextCharCode) const
{
	//FIXME
//	appDrawChar(outImage, m_pimpl, charcode);
//	outImage->yMirror();
	return getDistance(charcode, nextCharCode);
}

//-----------------------------------------------------------------------------

TPoint TFont::drawChar(TRasterCM32P &outImage, TPoint &unused, int inkId, wchar_t charcode, wchar_t nextCharCode) const
{
	//FIXME
	return TPoint(0, 0);
}

//-----------------------------------------------------------------------------

TPoint TFont::getDistance(wchar_t firstChar, wchar_t secondChar) const
{
	//FIXME
	return TPoint(0, 0);
}

//-----------------------------------------------------------------------------

int TFont::getMaxHeight() const
{
	QFontMetrics metrics(m_pimpl->m_font);
	return metrics.ascent() - metrics.descent();
}

//-----------------------------------------------------------------------------

int TFont::getMaxWidth() const
{
	QFontMetrics metrics(m_pimpl->m_font);
	return metrics.maxWidth();
}
//-----------------------------------------------------------------------------

int TFont::getLineAscender() const
{
	QFontMetrics metrics(m_pimpl->m_font);
	return metrics.ascent();
}

//-----------------------------------------------------------------------------

int TFont::getLineDescender() const
{
	QFontMetrics metrics(m_pimpl->m_font);
	return metrics.descent();
}

//-----------------------------------------------------------------------------

bool TFont::hasKerning() const
{
	return m_pimpl->m_font.kerning();
}

//-----------------------------------------------------------------------------

bool TFont::hasVertical() const
{
	//FIXME
	return false;
}

//-----------------------------------------------------------------------------

//=============================================================================
//====================      TFontManager  =====================================
//=============================================================================

//---------------------------------------------------------

struct TFontManager::Impl {
	QFontDatabase *m_qfontdb;
	bool m_loaded;

	TFont *m_currentFont;
	wstring m_currentFamily;
	wstring m_currentTypeface;
	int m_size;


	// this option is set by library user when he wants to write vertically.
	// In this implementation, if m_vertical is true and the font
	// has the @-version, the library use it.
	bool m_vertical;

	Impl()
		: m_qfontdb(NULL), m_loaded(false), m_currentFont(0), m_size(0),
		  m_vertical(false)
	{
	}
	~Impl()
	{
		delete m_qfontdb;
	}
};

//---------------------------------------------------------

TFontManager::TFontManager()
{
	m_pimpl = new TFontManager::Impl();
}

//---------------------------------------------------------

TFontManager::~TFontManager()
{
	delete m_pimpl;
}

//---------------------------------------------------------

TFontManager *TFontManager::instance()
{
	static TFontManager theManager;
	return &theManager;
}

//---------------------------------------------------------

void TFontManager::loadFontNames()
{
	if (m_pimpl->m_loaded)
		return;

	m_pimpl->m_qfontdb = new QFontDatabase;

	if (m_pimpl->m_qfontdb->families().empty())
		throw TFontLibraryLoadingError();

	m_pimpl->m_loaded = true;
}

//---------------------------------------------------------

void TFontManager::setFamily(const wstring family)
{
	if (m_pimpl->m_currentFamily == family)
		return;

	QString qFamily = QString::fromStdWString(family);
	QStringList families = m_pimpl->m_qfontdb->families();
	if (!families.contains(qFamily))
		throw TFontCreationError();

	m_pimpl->m_currentFamily = family;

	// XXX: if current style is not valid for family, reset it?
	// doing so asserts when chosing a font in the GUI
/*
	QStringList styles = m_pimpl->m_qfontdb->styles(qFamily);
	if (styles.contains(QString::fromStdWString(m_pimpl->m_currentTypeface)))
		m_pimpl->m_currentTypeface = L"";
*/
	delete m_pimpl->m_currentFont;
	m_pimpl->m_currentFont = new TFont(m_pimpl->m_currentFamily,
		m_pimpl->m_currentTypeface, m_pimpl->m_size);
}

//---------------------------------------------------------

void TFontManager::setTypeface(const wstring typeface)
{
	if (m_pimpl->m_currentTypeface == typeface)
		return;

	QString qTypeface = QString::fromStdWString(typeface);
	QStringList styles = m_pimpl->m_qfontdb->styles(QString::fromStdWString(m_pimpl->m_currentFamily));
	if (!styles.contains(qTypeface))
		throw TFontCreationError();

	m_pimpl->m_currentTypeface = typeface;

	delete m_pimpl->m_currentFont;
	m_pimpl->m_currentFont = new TFont(m_pimpl->m_currentFamily,
		m_pimpl->m_currentTypeface, m_pimpl->m_size);
}

//---------------------------------------------------------

void TFontManager::setSize(int size)
{
	if (m_pimpl->m_size == size)
		return;
	m_pimpl->m_size = size;
	delete m_pimpl->m_currentFont;
	m_pimpl->m_currentFont = new TFont(m_pimpl->m_currentFamily,
		m_pimpl->m_currentTypeface, m_pimpl->m_size);
}

//---------------------------------------------------------

wstring TFontManager::getCurrentFamily() const
{
	return m_pimpl->m_currentFamily;
}

//---------------------------------------------------------

wstring TFontManager::getCurrentTypeface() const
{
	return m_pimpl->m_currentTypeface;
}

//---------------------------------------------------------

TFont *TFontManager::getCurrentFont()
{
	if (m_pimpl->m_currentFont)
		return m_pimpl->m_currentFont;

	if (!m_pimpl->m_currentFont)
		loadFontNames();

	assert(!m_pimpl->m_qfontdb->families().empty());
	setFamily(m_pimpl->m_qfontdb->families().first().toStdWString());

	return m_pimpl->m_currentFont;
}

//---------------------------------------------------------

void TFontManager::getAllFamilies(vector<wstring> &families) const
{
	QStringList qFamilies = m_pimpl->m_qfontdb->families();

	families.clear();
	families.reserve(qFamilies.count());

	QStringList::const_iterator it = qFamilies.begin();
	for (; it != qFamilies.end(); ++it) {
		families.push_back(it->toStdWString());
	}
}

//---------------------------------------------------------

void TFontManager::getAllTypefaces(vector<wstring> &typefaces) const
{
	typefaces.clear();

	QStringList qStyles = m_pimpl->m_qfontdb->styles(QString::fromStdWString(m_pimpl->m_currentFamily));

	if (qStyles.empty())
		return;

	typefaces.reserve(qStyles.count());
	QStringList::const_iterator it_typeface = qStyles.begin();
	for (; it_typeface != qStyles.end(); ++it_typeface) {
		typefaces.push_back(it_typeface->toStdWString());
	}
}

//---------------------------------------------------------

void TFontManager::setVertical(bool vertical) {}

//---------------------------------------------------------


