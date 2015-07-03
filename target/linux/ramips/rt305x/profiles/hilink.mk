#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/HLK_RM04
	NAME:=HiLink HLK-RM04
	PACKAGES:=kmod-ledtrig-netdev kmod-ledtrig-timer
endef

define Profile/HLK_RM04/Description
	Package set for HiLink HLK-RM04 Module
endef

$(eval $(call Profile,HLK_RM04))
