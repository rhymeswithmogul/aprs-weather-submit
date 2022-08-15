# Copyright 1999-2022 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=8
DESCRIPTION="Manually submit weather data to the APRS network."
HOMEPAGE="https://github.com/rhymeswithmogul/${PN}"
SRC_URI="https://github.com/rhymeswithmogul/${PN}/archive/v${PV}.tar.gz" -> "${PF}.tar.gz"
LICENSE="AGPL-3 AGPL-3+"
SLOT="0"
KEYWORDS="amd64 ~ppc ~ppc-macos ppc64 s390x x86"
IUSE="+aprs-is debug"

src_configure() {
	econf \
		$(use_enable aprs-is) \
		$(use_enable debug)
}
