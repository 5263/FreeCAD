/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <werner.wm.mayer@gmx.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#ifndef GUI_TRANSLATOR_H
#define GUI_TRANSLATOR_H

#ifndef __Qt4All__
# include <Gui/Qt4All.h>
#endif

#include <string>
#include <map>
#include <list>

namespace Gui {

typedef std::list<std::string> TStringList;

/**
 * The Translator class uses Qt's QTranslator objects to change the language of the application
 * on the fly.
 * For more details see the \link i18n.html Internationalization with FreeCAD \endlink
 * documentation.
 *
 * \author Werner Mayer
 */
class GuiExport Translator : public QObject
{
    Q_OBJECT

public:
    /** @name singleton stuff */
    //@{
    /// Creates an instance
    static Translator* instance(void);
    /// Destroys the instance
    static void destruct (void);
    //@}

    /** Activates the specified language \a lang if available. */
    void activateLanguage (const char* lang);
    /* Reloads the translators */
    void refresh();
    /** Returns the currently installed language. If no language is installed an empty string is returned. */
    std::string activeLanguage() const;
    /** Returns a list of supported languages. */
    TStringList supportedLanguages() const;

private:
    Translator();
    ~Translator();
    void removeTranslators();

    static Translator* _pcSingleton;
    std::string activatedLanguage; /**< Active language */
    std::map<std::string, std::string> mapLanguageTopLevelDomain;
    std::list<QTranslator*> translators; /**< A list of all created translators */
};

} // namespace Gui

#endif // GUI_TRANSLATOR_H
