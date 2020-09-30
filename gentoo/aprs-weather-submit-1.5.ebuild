# Copyright 1999-2020 Gentoo Authors
# Distributed under the terms of the GNU General Public License v2

EAPI=7
DESCRIPTION="Manually submit weather data to the APRS network."
HOMEPAGE="https://github.com/rhymeswithmogul/aprs-weather-submit"
SRC_URI="https://github.com/rhymeswithmogul/aprs-weather-submit/archive/v1.5.tar.gz" -> "aprs-weather-submit_1.5.tar.gz"
LICENSE="AGPL-3 AGPL-3+"
SLOT="0"
KEYWORDS="~amd64"
IUSE="+aprs-is"

src_configure() {
	# ...and we do need different behavior, only to define our sole USE
	# flag, aprs-is for APRS-IS functionality (enabled by default).
	econf $(use_enable aprs-is)
}
