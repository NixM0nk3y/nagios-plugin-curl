# creates distribution tarball
#
# requires:
# - PACKAGE_NAME
# - PACKAGE_VERSION
#
# provides:
# - target 'dist'

.PHONY: dist
dist: distclean
	test -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar || rm -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar
	test -d /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION) || rm -rf /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION)
	find . -type f | grep -v .svn | \
		xargs tar cf /tmp/pre
	mkdir /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION)
	cd /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION) && \
		tar xf ../pre && rm -f ../pre && cd .. && \
		tar cvf /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION).tar $(PACKAGE_NAME)-$(PACKAGE_VERSION)
	rm -rf /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION)
	mv -f /tmp/$(PACKAGE_NAME)-$(PACKAGE_VERSION).tar .
