#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H
#include <QAbstractItemDelegate>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QSize>
#include <QStandardItemModel>

class listviewdelegate :  public QAbstractItemDelegate
{

private:
    int d_radius;
    int d_toppadding;
    int d_bottompadding;
    int d_leftpadding;
    int d_rightpadding;
    int d_verticalmargin;
    int d_horizontalmargin;
    int d_pointrwidth;
    int d_pointerheight;
    float d_widthfraction;

public:
    inline listviewdelegate(QObject * parent = nullptr);
    QString font;
    int size;
    void SetFont(QString f){font = f;}
    void setSize(int s){size = s;}



protected:
    inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const ;
};

inline listviewdelegate::listviewdelegate(QObject * parent) : QAbstractItemDelegate(parent),
    d_radius(5),
    d_toppadding(5),
    d_bottompadding(3),
    d_leftpadding(5),
    d_rightpadding(5),
    d_verticalmargin(15),
    d_horizontalmargin(10),
    d_pointrwidth(10),
    d_pointerheight(17),
    d_widthfraction(0.7)
{}

inline void listviewdelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QTextDocument bodydoc;
    QTextOption textOption(bodydoc.defaultTextOption());
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    bodydoc.setDefaultFont(QFont(font,size));
    QString bodytext(index.data(Qt::DisplayRole).toString());
    bodydoc.setHtml(bodytext);

    qreal contentswidth = option.rect.width() * d_widthfraction - d_horizontalmargin - d_pointrwidth - d_leftpadding - d_rightpadding;
    bodydoc.setTextWidth(contentswidth);
    qreal bodyheight = bodydoc.size().height();

    bool outgoing = index.data(Qt::UserRole + 1).toString() == "outgoing";
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(option.rect.left() + d_horizontalmargin,option.rect.top() + ((index.row() == 0) ? d_verticalmargin : 0));
    QColor bgcolor("#55ff7f");
    if(outgoing) bgcolor = "#DDDDDD";

    QPainterPath pointie;

    pointie.moveTo(0,bodyheight + d_toppadding + d_bottompadding);
    pointie.lineTo(0 + contentswidth + d_pointrwidth + d_leftpadding + d_rightpadding - d_radius, bodyheight + d_toppadding + d_bottompadding);
    pointie.arcTo(0 + contentswidth + d_pointrwidth + d_leftpadding + d_rightpadding -2 * d_radius,bodyheight + d_toppadding + d_bottompadding -2 * d_radius, d_radius,2* d_radius,270,90 *2);


    pointie.lineTo(0 + contentswidth + d_pointrwidth + d_leftpadding + d_rightpadding, 0 + d_radius);
    pointie.arcTo(0 + contentswidth + d_pointrwidth  + d_leftpadding + d_rightpadding -2 * d_radius, 0 , d_radius , 2 * d_radius, 0 , 90 * 2);

    pointie.lineTo(0 + d_pointrwidth + d_radius , 0);
    pointie.arcTo( 0 + d_pointrwidth , 0 , 2 * d_radius , 2 * d_radius , 90 , 90);

    pointie.lineTo(0 + d_pointrwidth , bodyheight + d_toppadding + d_bottompadding - d_pointerheight);
    pointie.closeSubpath();

    if(outgoing){
        painter->translate(option.rect.width() - pointie.boundingRect().width() - d_horizontalmargin - d_pointrwidth , 0);
        painter->translate(pointie.boundingRect().center());
        painter->rotate(180);
        painter->translate(-pointie.boundingRect().center());
    }

    painter->setPen(QPen(bgcolor));
    painter->drawPath(pointie);
    painter->fillPath(pointie,QBrush(bgcolor));

    if(outgoing){
        painter->translate(pointie.boundingRect().center());
        painter->rotate(-180);
        painter->translate(-pointie.boundingRect().center());
    }

    QAbstractTextDocumentLayout::PaintContext ctx;
    if(outgoing) ctx.palette.setColor(QPalette::Text,QColor("black"));
    else ctx.palette.setColor(QPalette::Text, QColor("black"));

    painter->translate((outgoing ? 0 : d_pointrwidth) + d_leftpadding , 0);
    bodydoc.documentLayout()->draw(painter,ctx);
    painter->restore();

}

inline QSize listviewdelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
    QTextDocument bodydoc;
    QTextOption textOption(bodydoc.defaultTextOption());
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    bodydoc.setDefaultFont(QFont(font,size));
    QString bodytext(index.data(Qt::DisplayRole).toString());
    bodydoc.setHtml(bodytext);

    qreal contentswidth = option.rect.width() * d_widthfraction - d_horizontalmargin - d_pointrwidth - d_leftpadding - d_rightpadding;

    bodydoc.setTextWidth(contentswidth);
    QSize size(bodydoc.idealWidth() + d_horizontalmargin + d_pointrwidth + d_leftpadding + d_rightpadding , bodydoc.size().height() + d_bottompadding + d_toppadding + d_verticalmargin + 1);

    if(index.row() == 0) size += QSize(0,d_verticalmargin);

    return size;

}


#endif // LISTVIEWDELEGATE_H
