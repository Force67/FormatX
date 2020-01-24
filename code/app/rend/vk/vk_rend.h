#pragma once

// Copyright (C) 2019-2020 Force67

#include <QGuiApplication>
#include <QVulkanInstance>
#include <QVulkanWindow>

#include "../rend.h"

class vkBackend : public rendInterface, public QVulkanWindow
{
public:
	vkBackend(QWindow* parent) :
		QVulkanWindow(parent)
	{}

	bool create() override;

	inline QVulkanInstance& vkInstance() {
		return vkInst;
	}

private:
	QVulkanWindowRenderer* createRenderer() override;
	QVulkanInstance vkInst;
};