# Copyright 1999-2022 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=8
DESCRIPTION="Manually submit weather data to the APRS network."
HOMEPAGE="https://github.com/rhymeswithmogul/${PN}"
SRC_URI="https://github.com/rhymeswithmogul/${PN}/archive/v${PV}.tar.gz" -> "${PF}.tar.gz"
LICENSE="AGPL-3 AGPL-3+"
SLOT="0"

# This app has been tested on amd64, arm, arm64, ppc, ppc64, riscv, s390x, and x86.
KEYWORDS="~alpha amd64 arm arm64 ~hppa ~ia64 ~mips ppc ~ppc-macos ppc64 riscv s390x ~sparc x86"

# USE flags:
#  - aprs-is: compile with APRS-IS support (enabled by default)
#  - minimal: compile without debugging support (disabled by default)
IUSE="+aprs-is -minimal"


inherit autotools

src_configure() {
	default

	if use minimal; then
		econf --disable-debugging
	fi

	econf $(use_enable aprs-is)
}
