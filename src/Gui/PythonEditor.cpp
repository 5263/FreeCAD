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


#include "PreCompiled.h"

#ifndef _PreComp_
# include <qregexp.h>
# include <qtextedit.h>
# include <qsyntaxhighlighter.h>
# include <qclipboard.h>
# include <qmessagebox.h>
# include <qfile.h>
# include <qfiledialog.h>
#endif

#include "PythonEditor.h"
#include "Application.h"
#include "DlgEditorImp.h"
#include "../Base/Interpreter.h"
#include "../Base/Exception.h"
#include "../Base/Parameter.h"

using namespace Gui;
using Gui::Dialog::GetDefCol;

/**
 *  Constructs a PythonWindow which is a child of 'parent', with the
 *  name 'name' and installs the Python syntax highlighter.
 */
PythonWindow::PythonWindow(QWidget *parent,const char *name)
    : QTextEdit(parent, name)
{
  nInsertTabs = 0;
  pythonSyntax = new PythonSyntaxHighlighter(this);
}

/** Destroys the object and frees any allocated resources */
PythonWindow::~PythonWindow()
{
  delete pythonSyntax;
}

void PythonWindow::keyPressEvent(QKeyEvent * e)
{
  QTextEdit::keyPressEvent(e);
//
//  switch (e->key())
//  {
//    case Key_Colon:
//      nInsertTabs++;
//      break;
//
//    case Key_Return:
//    case Key_Enter:
//      if (nInsertTabs > 0)
//      {
//        for (int i=0; i<nInsertTabs;i++)
//        {
//          insert("\t");
//        }
//      }
//      else
//      {
//        try{
//        }
//        catch(...)
//        {
//        }
//        nInsertTabs=0;
//      }
//      break;
//  }
}

// ------------------------------------------------------------------------

/**
 *  Constructs a PythonEditor which is a child of 'parent', with the
 *  name 'name'. 
 */
PythonEditor::PythonEditor(QWidget *parent,const char *name)
    : PythonWindow(parent, name)
{
}

/** Destroys the object and frees any allocated resources */
PythonEditor::~PythonEditor()
{
}

// ------------------------------------------------------------------------

/**
 *  Constructs a PythonConsole which is a child of 'parent', with the
 *  name 'name'. 
 */
PythonConsole::PythonConsole(QWidget *parent,const char *name)
    : PythonWindow(parent, name), lastPara(-1)
{
  zoomIn(2);
#ifdef FC_OS_LINUX
  QFont serifFont( "Courier", 15, QFont::Normal );
#else
  QFont serifFont( "Courier", 10, QFont::Normal );
#endif
  setFont(serifFont);

  setTabStopWidth(32);
}

/** Destroys the object and frees any allocated resources */
PythonConsole::~PythonConsole()
{
}

/**
 * Checks the input of the console to make the correct indentations.
 * After a command is prompted completely the Python interpreter is started.
 */
void PythonConsole::keyPressEvent(QKeyEvent * e)
{
  PythonWindow::keyPressEvent(e);

  switch (e->key())
  {
  case Key_Return:
  case Key_Enter:
    int para; int idx;
    getCursorPosition(&para, &idx);
    QString txt = text(para-1);
    if (txt.isEmpty() || txt == " ")
    {
      bool bMute = GuiConsoleObserver::bMute;

      try
      {
        // check if any python command was already launched
        int prev = lastPara > -1 ? lastPara : 0;
        QString cmd;
        // build the python command
        for (int i=prev; i<para; i++)
        {
          cmd += text(i);
          cmd += "\n";
        }
        lastPara = para;

        // launch the command now
        GuiConsoleObserver::bMute = true;
        if (!cmd.isEmpty())
          Base::Interpreter().Launch(cmd.latin1());
      }
      catch (const Base::Exception& exc)
      {
        setColor(Qt::red);
        pythonSyntax->highlightError(true);
        insertAt( exc.what(), lastPara-1, 0);
        pythonSyntax->highlightError(false);
      }

      GuiConsoleObserver::bMute = bMute;
    }
    break;
  }

  // cursor must not be inside the text of the last command
  int para; int idx;
  getCursorPosition(&para, &idx);
  if (para < lastPara)
  {
    setCursorPosition(lastPara, idx);
  }
}

// ------------------------------------------------------------------------

namespace Gui {
class PythonSyntaxHighlighterP
{
public:
  PythonSyntaxHighlighterP() : hlError(false)
  {
    keywords << "and" << "assert" << "break" << "class" << "continue" << "def" << "del" <<
    "elif" << "else" << "except" << "exec" << "finally" << "for" << "from" << "global" <<
    "if" << "import" << "in" << "is" << "lambda" << "None" << "not" << "or" << "pass" << "print" <<
    "raise" << "return" << "try" << "while" << "yield";

    // set colors
    FCParameterGrp::handle hPrefGrp = GetApplication().GetUserParameter().GetGroup("BaseApp");
    hPrefGrp = hPrefGrp->GetGroup("Windows")->GetGroup("Editor");

    long c;
    c = hPrefGrp->GetInt("Text", GetDefCol().color("Text"));
    cNormalText.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Comment", GetDefCol().color("Comment"));
    cComment.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Block comment", GetDefCol().color("Block comment"));
    cBlockcomment.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Number", GetDefCol().color("Number"));
    cNumber.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("String", GetDefCol().color("String"));
    cLiteral.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Keyword", GetDefCol().color("Keyword"));
    cKeyword.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Class name", GetDefCol().color("Class name"));
    cClassName.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Define name", GetDefCol().color("Define name"));
    cDefineName.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
    c = hPrefGrp->GetInt("Operator", GetDefCol().color("Operator"));
    cOperator.setRgb(c & 0xff, (c >> 8) & 0xff, (c >> 16) & 0xff);
  }

  QStringList keywords;
  QString blockComment;
  bool hlError;
  QColor cNormalText, cComment, cBlockcomment, cLiteral, cNumber,
  cOperator, cKeyword, cClassName, cDefineName;
};
} // namespace Gui

/**
 * Constructs a Python syntax highlighter.
 */
PythonSyntaxHighlighter::PythonSyntaxHighlighter(QTextEdit* edit)
    : QSyntaxHighlighter(edit)
{
  d = new PythonSyntaxHighlighterP;
}

/** Destroys this object. */
PythonSyntaxHighlighter::~PythonSyntaxHighlighter()
{
  delete d;
}

/**
 * If \a b is set to true the following input to the editor is highlighted as error.
 */
void PythonSyntaxHighlighter::highlightError (bool b)
{
  d->hlError = b;
}

/**
 * Highlights a part of the text \a txt as block comment.
 */
int PythonSyntaxHighlighter::highlightBlockComment( const QString& txt, int& from, int endStateOfLastPara )
{
  QRegExp rx("(\"\"\"|''')");
  int pos = txt.find(rx, from);
  // if this is part of a block search for the closing tag
  if (endStateOfLastPara == (int)Blockcomment)
    pos = txt.find(d->blockComment, from);
  else if (pos > -1)
    d->blockComment = rx.cap(1);

  // whole line is a comment
  if ( pos == -1 && endStateOfLastPara == (int)Blockcomment)
  {
    setFormat(0, txt.length(), d->cBlockcomment);
  }
  // tag for block comments found
  else if ( pos > -1)
  {
    int begin = pos;

    // not part of a block comment
    if ( endStateOfLastPara != (int)Blockcomment )
    {
      // search also for the closing tag
      int end = txt.find(d->blockComment, begin+3);

      // all between "begin" and "end" is a comment
      if ( end > -1 )
      {
        int len = end - begin + 3;
        setFormat(begin, len, d->cBlockcomment);
        from = end+3;
      }
      // all after "begin" is a comment
      else
      {
        int len = txt.length() - begin;
        setFormat(begin, len, d->cBlockcomment);
        from = txt.length();
        endStateOfLastPara = (int)Blockcomment;
      }
    }
    // previous line is part of a block comment
    else
    {
      int len = begin + 3;
      setFormat(0, len, d->cBlockcomment);
      from = begin+3;
      endStateOfLastPara = (int)Normal;
    }
  }

  return endStateOfLastPara;
}

/**
 * Highlights a part of the text \a txt as string literal.
 */
int PythonSyntaxHighlighter::highlightLiteral( const QString& txt, int& from, int endStateOfLastPara )
{
  QRegExp rx("(\"|')");
  int pos = txt.find(rx, from);
  QString pat = rx.cap(1);
  if (pos > -1)
  {
    int begin = pos;

    // search also for the closing tag
    int end = txt.find(pat, begin+1);

    if (end > -1)
    {
      int len = end - begin + 1;
      setFormat(begin, len, d->cLiteral);
      from = end+1;
      endStateOfLastPara = (int)Normal;
    }
    else
    {
      // whole line is a literal
      int len = txt.length()-begin;
      setFormat(begin, len, d->cLiteral);
      from = txt.length();
      endStateOfLastPara = (int)Literal;
    }
  }

  // no blocks for literals allowed
  return (int)endStateOfLastPara;
}

/**
 * Highlights a part of the text \a txt as comment.
 */
int PythonSyntaxHighlighter::highlightComment( const QString& txt, int& from, int endStateOfLastPara )
{
  // check for comments
  int pos = txt.find("#", from);
  if (pos != -1)
  {
    int len = txt.length()-pos;
    setFormat(pos, len, d->cComment);
    endStateOfLastPara = (int)Comment;
  }

  return (int)endStateOfLastPara;
}

/**
 * Highlights a part of the text \a txt as normal text.
 */
int PythonSyntaxHighlighter::highlightNormalText( const QString& txt, int& from, int endStateOfLastPara )
{
  // colourize everything black
  setFormat(from, txt.length()-from, d->cNormalText);

  return 0;
}

/**
 * Highlights all keywords of the text \a txt.
 */
int PythonSyntaxHighlighter::highlightKeyword( const QString& txt, int& from, int endStateOfLastPara )
{
  // search for all keywords to colourize
  for (QStringList::Iterator it = d->keywords.begin(); it != d->keywords.end(); ++it)
  {
    QRegExp keyw(QString("\\b%1\\b").arg(*it));
    int pos = txt.find(keyw, from);
    while (pos > -1)
    {
      QFont font = textEdit()->currentFont();
      font.setBold( true );
      setFormat(pos, (*it).length(), font, d->cKeyword);

      // make next word bold
      if ( (*it) == "def" || (*it) == "class")
      {
        QRegExp word("\\b[A-Za-z0-9]+\\b");
        int wd = txt.find(word, pos+(*it).length());
        if ( wd > -1)
        {
          int len = word.matchedLength();
          QFont font = textEdit()->currentFont();
          font.setBold( true );
          if ((*it) == "def")
            setFormat(wd, len, font, d->cDefineName);
          else // *it == "class"
            setFormat(wd, len, font, d->cClassName);
        }
      }

      pos = txt.find(keyw, pos+(*it).length());
    }
  }

  return 0;
}

/**
 * Highlights all operators of the text \a txt.
 */
int PythonSyntaxHighlighter::highlightOperator( const QString& txt, int& from, int endStateOfLastPara )
{
  QRegExp rx( "[\\[\\]\\{\\}\\(\\)\\+\\*\\-/<>]" );
  int pos = rx.search( txt, from );
  while ( pos > -1 )
  {
    int len = rx.matchedLength();
    setFormat(pos, len, d->cOperator);
    pos = rx.search( txt, pos+len );
  }

  return 0;
}

/**
 * Highlights all numbers of the text \a txt.
 */
int PythonSyntaxHighlighter::highlightNumber( const QString& txt, int& from, int endStateOfLastPara )
{
  QRegExp rx( "\\b(\\d+)\\b" );
  int pos = rx.search( txt, from );
  while ( pos > -1 )
  {
    int len = rx.matchedLength();
    setFormat(pos, len, d->cNumber);
    pos = rx.search( txt, pos+len );
  }

  return 0;
}

/**
 * Detects all kinds of text to highlight them in the correct color.
 */
int PythonSyntaxHighlighter::highlightParagraph ( const QString & text, int endStateOfLastPara )
{
  QString txt(text);
  int from = 0;

  // highlight as error
  if ( d->hlError )
  {
    QFont font = textEdit()->currentFont();
    font.setItalic( true );
    setFormat(0, txt.length(), font, Qt::red);
    return 0;
  }

  // colourize all keywords, operators, numbers, normal text
  highlightNormalText(txt, from, endStateOfLastPara);
  highlightKeyword   (txt, from, endStateOfLastPara);
  highlightOperator  (txt, from, endStateOfLastPara);
  highlightNumber    (txt, from, endStateOfLastPara);

  QString comnt("#");
  QRegExp blkcm("(\"\"\"|''')");
  QRegExp strlt("(\"|')");
  while ( from < (int)txt.length() && txt != " ")
  {
    if ( endStateOfLastPara == int(Blockcomment) )
    {
      // search for closing block comments
      endStateOfLastPara = highlightBlockComment (txt, from, endStateOfLastPara);
      if (endStateOfLastPara > 0) return endStateOfLastPara;
    }
    else
    {
      // search for the first tag
      int com = txt.find(comnt, from);
      if (com < 0) com = txt.length();

      int bcm = txt.find(blkcm, from);
      if (bcm < 0) bcm = txt.length();

      int str = txt.find(strlt, from);
      if (str < 0) str = txt.length();

      // comment
      if (com < bcm && com < str)
      {
        from = com;
        highlightComment (txt, from, endStateOfLastPara);
        return 0;
      }
      // block comment
      else if (bcm < com && bcm <= str)
      {
        from = bcm;
        endStateOfLastPara = highlightBlockComment (txt, from, endStateOfLastPara);
        if (endStateOfLastPara > 0) return endStateOfLastPara;
      }
      // string literal
      else if (str < com && str < bcm)
      {
        from = str;
        endStateOfLastPara = highlightLiteral (txt, from, endStateOfLastPara);
        if (endStateOfLastPara > 0) return 0;
      }
      // no tag found
      else
      {
        from = txt.length();
      }
    }
  }

  return 0;
}

// ------------------------------------------------------------------------

/**
 *  Constructs a PythonEditView which is a child of 'parent', with the
 *  name 'name'.
 */
PythonEditView::PythonEditView( QWidget* parent, const char* name)
    : FCView(0,parent, name)
{
  setCaption("Editor");
  setTabCaption("Editor");
  resize( 400, 300 );

  textEdit = new PythonEditor(this);
  textEdit->setWordWrap( QTextEdit::NoWrap );

  QGridLayout *layout = new QGridLayout(this,0,0);
  layout->addWidget(textEdit,0,0);
}

/** Destroys the object and frees any allocated resources */
PythonEditView::~PythonEditView()
{
  delete textEdit;
}

/**
 * Runs the action specified by \a pMsg.
 */
bool PythonEditView::OnMsg(const char* pMsg)
{
  if (strcmp(pMsg,"Save")==0){
    save();
    return true;
  }
  if (strcmp(pMsg,"SaveAs")==0){
    saveAs();
    return true;
  }
  if (strcmp(pMsg,"Cut")==0){
    cut();
    return true;
  }
  if (strcmp(pMsg,"Copy")==0){
    copy();
    return true;
  }
  if (strcmp(pMsg,"Paste")==0){
    paste();
    return true;
  }
  if (strcmp(pMsg,"Undo")==0){
    undo();
    return true;
  }
  if (strcmp(pMsg,"Redo")==0){
    redo();
    return true;
  }

  Base::Console().Log("PythonEditView::OnMsg() unhandled \"%s\"\n",pMsg);
  return false;
}

/**
 * Checks if the action \a pMsg is available. This is for enabling/disabling
 * the corresponding buttons or menu items for this action.
 */
bool PythonEditView::OnHasMsg(const char* pMsg)
{
  if (strcmp(pMsg,"Save")==0)  return true;
  if (strcmp(pMsg,"Print")==0) return true;
  if (strcmp(pMsg,"Cut")==0)
  {
    bool canWrite = !textEdit->isReadOnly();
    return (canWrite && (textEdit->hasSelectedText()));
  }
  if (strcmp(pMsg,"Copy")==0)
  {
    return ( textEdit->hasSelectedText() );
  }
  if (strcmp(pMsg,"Paste")==0)
  {
    QClipboard *cb = QApplication::clipboard();
    QString text;

    // Copy text from the clipboard (paste)
    text = cb->text();

    bool canWrite = !textEdit->isReadOnly();
    return ( !text.isEmpty() && canWrite );
  }
  if (strcmp(pMsg,"Undo")==0)
  {
    return textEdit->isUndoAvailable ();
  }
  if (strcmp(pMsg,"Redo")==0)
  {
    return textEdit->isRedoAvailable ();
  }
  return false;
}

/** Checking on close state. */
bool PythonEditView::CanClose(void)
{
  if ( !textEdit->isModified() )
    return true;

  switch(QMessageBox::warning( this, "Unsaved document","Save file before close?","Yes","No","Cancel",0,2))
  {
  case 0:
    return save();
  case 1:
    return true;
  case 2:
    return false;
  default:
    return false;
  }
}

/**
 * Saves the changes of the editor to a file. If the input has not been saved yet before
 * a file dialog appears.
 */
bool PythonEditView::save()
{
  if ( !textEdit->isModified() )
    return true;

  // check if saved ever before
  if ( isSavedOnce() )
  {
    saveFile();
    return true;
  }
  else
  {
    return saveAs();
  }
}

/**
 * Saves the content of the editor to a file specified by the appearing file dialog.
 */
bool PythonEditView::saveAs(void)
{
  QString fn = QFileDialog::getSaveFileName(QString::null, "FreeCAD macro (*.FCMacro);;Python (*.py)", this);
  if (!fn.isEmpty())
  {
    _fileName = fn;
    saveFile();
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Opens a file specified by the appearing file dialog.
 */
bool PythonEditView::open(void)
{
  QString file = QFileDialog::getOpenFileName(QString::null, "Macro files (*.py *.FCMacro);;Python (*.py);;FreeCAD macro (*.FCMacro)", this);
  if ( file.isEmpty() )
    return false;

  openFile(file);
  setCaption(file);
  QString name = file.left(file.findRev('.'));
  setTabCaption(name);

  return true;
}

/**
 * Opens the file \a fileName.
 */
void PythonEditView::openFile (const QString& fileName)
{
  QString line;
  _fileName = fileName;
  QFile file(fileName);

  if( !file.open(IO_ReadOnly))
    return;

  QTextStream in(&file);
  QString text;

  while( !in.atEnd() ){
    line = in.readLine();
    text = line + "\n";
    textEdit->append(text);
  }

  file.close();

  textEdit->setModified(false);
}

/**
 * Copies the selected text to the clipboard and deletes it from the text edit.
 * If there is no selected text nothing happens.
 */
void PythonEditView::cut(void)
{
  textEdit->cut();
}

/**
 * Copies any selected text to the clipboard.
 */
void PythonEditView::copy(void)
{
  textEdit->copy();
}

/**
 * Pastes the text from the clipboard into the text edit at the current cursor position. 
 * If there is no text in the clipboard nothing happens.
 */
void PythonEditView::paste(void)
{
  textEdit->paste();
}

/**
 * Undoes the last operation.
 * If there is no operation to undo, i.e. there is no undo step in the undo/redo history, nothing happens.
 */
void PythonEditView::undo(void)
{
  textEdit->undo();
}

/**
 * Redoes the last operation.
 * If there is no operation to undo, i.e. there is no undo step in the undo/redo history, nothing happens.
 */
void PythonEditView::redo(void)
{
  textEdit->redo();
}

/**
 * \todo: Shows the printer dialog.
 */
void PythonEditView::Print(QPainter& cPrinter)
{
  // no printing yet ;-)
  assert(0);
}

/**
 * Saves the contents to a file.
 */
void PythonEditView::saveFile()
{
  QFile file(_fileName);
  if( !file.open(IO_WriteOnly))
    return;

  QTextStream out(&file);
  out << textEdit->text();
  file.close();

  textEdit->setModified(false);

  return;
}

/**
 * Checks if the contents has been saved once to a file.
 */
bool PythonEditView::isSavedOnce()
{
  return (!_fileName.isEmpty());
}
