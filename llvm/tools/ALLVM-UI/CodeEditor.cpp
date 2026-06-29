#include "CodeEditor.h"
#include <QPainter>
#include <QTextBlock>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);
    setWordWrapMode(QTextOption::NoWrap);
    setDarkMode(false);
}

void CodeEditor::setDarkMode(bool darkMode) {
    m_darkMode = darkMode;
    if (m_darkMode) {
        setStyleSheet(
            "QPlainTextEdit {"
            "  background-color: #111827;"
            "  color: #e5eefb;"
            "  border: 1px solid #334155;"
            "  border-radius: 12px;"
            "  font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', 'Monaco', monospace;"
            "  font-size: 13px;"
            "  line-height: 1.5;"
            "  padding: 8px;"
            "}"
        );
    } else {
        setStyleSheet(
            "QPlainTextEdit {"
            "  background-color: #ffffff;"
            "  color: #1f2937;"
            "  border: 1px solid #d7deea;"
            "  border-radius: 12px;"
            "  font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', 'Monaco', monospace;"
            "  font-size: 13px;"
            "  line-height: 1.5;"
            "  padding: 8px;"
            "}"
        );
    }
    highlightCurrentLine();
    lineNumberArea->update();
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = m_darkMode ? QColor(30, 41, 59) : QColor(241, 245, 249);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), m_darkMode ? QColor(15, 23, 42) : QColor(248, 250, 252));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(m_darkMode ? QColor(100, 116, 139) : QColor(148, 163, 184));
            painter.setFont(font());
            painter.drawText(0, top, lineNumberArea->width() - 6, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
