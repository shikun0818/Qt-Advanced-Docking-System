
#include "ads/FloatingWidget.h"

#include <QDebug>
#include <QBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QStyle>

#include "ads/ContainerWidget.h"
#include "ads/SectionTitleWidget.h"
#include "ads/SectionContentWidget.h"
#include "ads/Internal.h"

ADS_NAMESPACE_BEGIN

FloatingWidget::FloatingWidget(ContainerWidget* container, SectionContent::RefPtr sc, SectionTitleWidget* titleWidget, SectionContentWidget* contentWidget, QWidget* parent) :
	QWidget(parent, Qt::CustomizeWindowHint | Qt::Tool),
	_container(container),
	_content(sc),
	_titleWidget(titleWidget),
	_contentWidget(contentWidget)
{
	QBoxLayout* l = new QBoxLayout(QBoxLayout::TopToBottom);
	l->setContentsMargins(0, 0, 0, 0);
	l->setSpacing(0);
	setLayout(l);

	// Title + Controls
	_titleLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	_titleLayout->addWidget(titleWidget, 1);
	l->addLayout(_titleLayout, 0);
	titleWidget->setActiveTab(false);

	QPushButton* closeButton = new QPushButton();
	closeButton->setObjectName("closeButton");
	closeButton->setFlat(true);
	closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
	closeButton->setToolTip(tr("Close"));
	closeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	_titleLayout->addWidget(closeButton);
	//QObject::connect(closeButton, &QPushButton::clicked, this, &FloatingWidget::close);
	QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	// Content
	l->addWidget(contentWidget, 1);
	contentWidget->show();

//	_container->_floatingWidgets.append(this);
}

FloatingWidget::~FloatingWidget()
{
	qDebug() << Q_FUNC_INFO;
	_container->_floatings.removeAll(this); // Note: I don't like this here, but we have to remove it from list...
}

bool FloatingWidget::takeContent(InternalContentData& data)
{
	data.content = _content;
	data.titleWidget = _titleWidget;
	data.contentWidget = _contentWidget;

	_titleLayout->removeWidget(_titleWidget);
	_titleWidget = NULL;

	layout()->removeWidget(_contentWidget);
	_contentWidget = NULL;

	return true;
}

ADS_NAMESPACE_END