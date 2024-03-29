include $(top_srcdir)/build/autotools/Makefile.am.silent

UNIT_TESTS = \
	test-texture-slicing.c \
	test-texture-async.c \
	test-texture-material.c \
	test-events.c \
	test-scale.c \
        test-actors.c \
	test-shader-effects.c \
	test-depth.c \
	test-threads.c \
	test-script.c \
	test-grab.c \
	test-cogl-shader-arbfp.c \
	test-cogl-shader-glsl.c \
	test-animator.c \
	test-state.c \
	test-state-animator.c \
	test-fbo.c \
	test-multistage.c \
	test-cogl-primitives.c \
	test-cogl-tex-tile.c \
	test-cogl-tex-convert.c \
	test-cogl-tex-foreign.c \
	test-cogl-tex-getset.c \
	test-cogl-offscreen.c \
	test-cogl-tex-polygon.c \
	test-cogl-multitexture.c \
	test-stage-read-pixels.c \
	test-clip.c \
	test-paint-wrapper.c \
	test-texture-quality.c \
	test-layout.c \
        test-animation.c \
        test-easing.c \
        test-binding-pool.c \
        test-text.c \
        test-text-field.c \
	test-cairo-clock.c \
	test-cairo-flowers.c \
	test-cogl-vertex-buffer.c \
	test-bin-layout.c \
	test-flow-layout.c \
	test-box-layout.c \
	test-stage-sizing.c \
	test-drag.c \
	test-constraints.c \
	test-scrolling.c \
	test-swipe-action.c \
	test-cogl-point-sprites.c \
	test-table-layout.c \
	test-path-constraint.c \
	test-snap-constraint.c \
	test-state-script.c \
	test-drop.c \
	test-devices.c \
	test-actor.c \
	test-transitions.c \
	test-content.c \
	test-canvas.c

if X11_TESTS
UNIT_TESTS += test-pixmap.c
endif

if PIXBUF_TESTS
UNIT_TESTS += \
	test-image.c \
	test-image-box.c
endif

if OS_WIN32
SHEXT =
else
SHEXT = $(EXEEXT)
endif

# For convenience, this provides a way to easily run individual unit tests:
wrappers: stamp-test-interactive $(top_builddir)/build/win32/test-interactive-clutter.bat
	@true

stamp-test-interactive: Makefile
	@wrapper=$(abs_builddir)/wrapper.sh ; \
	chmod +x $$wrapper && \
	( echo "/stamp-test-interactive" ; \
	  echo "/stamp-test-unit-names" ; \
	  echo "/test-interactive" ; \
	  echo "/test-unit-names.h" ; \
	  echo "*.o" ; \
	  echo ".gitignore" ) > .gitignore ; \
	for i in $(UNIT_TESTS); \
	do \
		test_bin=$${i%*.c} ; \
		echo "  GEN    $$test_bin" ; \
		( echo "#!/bin/sh" ; \
		  echo "$$wrapper $$test_bin \$$@" \
		) > $$test_bin$(SHEXT) ; \
		chmod +x $$test_bin$(SHEXT) ; \
		echo "/$$test_bin$(SHEXT)" >> .gitignore ; \
	done \
	&& echo timestamp > $(@F)

$(top_builddir)/build/win32/test-interactive-clutter.bat: Makefile test-interactive$(EXEEXT)
	@echo "  GEN    test-interactive-clutter.bat" ; \
	for i in $(UNIT_TESTS); \
	do \
		case $$i in \
		test-pixmap.c|test-devices.c) ;; \
		*.c)	test_bin=$${i%*.c} \
			;; \
		esac; \
		( echo "test-interactive-clutter $$test_bin" ) > $$test_bin-clutter.bat ; \
		( echo "test-interactive-clutter $$test_bin" ) >> test-interactive-clutter.bat ; \
	done \
	&& mv -f *.bat $(top_builddir)/build/win32/

$(top_builddir)/build/win32/test-unit-names.h: test-unit-names.h
	@echo "  CP     "; cp test-unit-names.h $(top_builddir)/build/win32/

test-unit-names.h: stamp-test-unit-names
	@true

stamp-test-unit-names: Makefile
	@( echo "/* ** This file is autogenerated. Do not edit. ** */" ; \
	  echo "" ; \
	  echo "const char *test_unit_names[] = {" ) > test-unit-names.h ; \
	for i in $(UNIT_TESTS); \
	do \
		test_bin=$${i%*.c} ; \
		echo "  \"$$test_bin\"," >> test-unit-names.h ; \
	done \
	&& echo "};" >> test-unit-names.h \
	&& echo timestamp > $(@F)

clean-wrappers:
	@for i in $(UNIT_TESTS); \
	do \
		test_bin=$${i%*.c} ; \
		echo "  RM    $$test_bin"; \
		rm -f $$test_bin$(SHEXT); \
	done \
	&& rm -f $(top_builddir)/build/win32/*.bat \
	&& rm -f stamp-test-unit-names \
	&& rm -f stamp-test-interactive

.PHONY: wrappers clean-wrappers

INCLUDES = \
	-I$(top_srcdir)			\
	-I$(top_srcdir)/clutter 	\
	-I$(top_builddir)/clutter

common_ldadd = $(top_builddir)/clutter/libclutter-@CLUTTER_API_VERSION@.la

noinst_PROGRAMS = test-interactive

test_interactive_SOURCES = test-main.c test-unit-names.h $(UNIT_TESTS)
test_interactive_CFLAGS = $(CLUTTER_CFLAGS) $(GDK_PIXBUF_CFLAGS)
test_interactive_CPPFLAGS = \
	-DTESTS_DATADIR=\""$(abs_top_srcdir)/tests/data"\" \
	-DG_DISABLE_SINGLE_INCLUDES \
	-DGLIB_DISABLE_DEPRECATION_WARNINGS \
	-DCLUTTER_DISABLE_DEPRECATION_WARNINGS
test_interactive_LDFLAGS = -export-dynamic
test_interactive_LDADD = $(CLUTTER_LIBS) $(GDK_PIXBUF_LIBS) $(common_ldadd) -lm

EXTRA_DIST = \
	wrapper.sh.in			\
	$(top_builddir)/build/win32/test-unit-names.h

DISTCLEANFILES = wrapper.sh .gitignore test-unit-names.h

BUILT_SOURCES = test-unit-names.h wrappers

dist-hook: $(top_builddir)/build/win32/vs9/test-interactive-clutter.vcproj $(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj $(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj.filters

$(top_builddir)/build/win32/vs9/test-interactive-clutter.vcproj: $(top_srcdir)/build/win32/vs9/test-interactive-clutter.vcprojin
	for F in $(test_interactive_SOURCES); do \
		case $$F in \
		test-pixmap.c|test-devices.c) ;; \
		*.c)	echo '   <File RelativePath="..\..\..\tests\interactive\'$$F'" />' \
			;; \
		esac; \
	done > testinteractive.sourcefiles
	$(CPP) -P - <$(top_srcdir)/build/win32/vs9/test-interactive-clutter.vcprojin >$@
	rm -f testinteractive.sourcefiles

$(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj: $(top_srcdir)/build/win32/vs10/test-interactive-clutter.vcxprojin
	for F in $(test_interactive_SOURCES); do \
		case $$F in \
		test-pixmap.c|test-devices.c) ;; \
		*.c)	echo '    <ClCompile Include="..\..\..\tests\interactive\'$$F'" />' \
			;; \
		esac; \
	done > testinteractive.vs10.sourcefiles
	$(CPP) -P - <$(top_srcdir)/build/win32/vs10/test-interactive-clutter.vcxprojin >$@
	rm -f testinteractive.vs10.sourcefiles

$(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj.filters: $(top_srcdir)/build/win32/vs10/test-interactive-clutter.vcxproj.filtersin
	for F in $(test_interactive_SOURCES); do \
		case $$F in \
		test-pixmap.c|test-devices.c) ;; \
		*.c)	echo '    <ClCompile Include="..\..\..\tests\interactive\'$$F'"><Filter>Sources</Filter></ClCompile>' \
			;; \
		esac; \
	done > testinteractive.vs10.sourcefiles.filters
	$(CPP) -P - <$(top_srcdir)/build/win32/vs10/test-interactive-clutter.vcxproj.filtersin >$@
	rm -f testinteractive.vs10.sourcefiles.filters

# Let the VS9/VS10 Project files be cleared out before they are re-expanded...
DISTCLEANFILES += \
	$(top_builddir)/build/win32/test-unit-names.h \
	$(top_builddir)/build/win32/vs9/test-interactive-clutter.vcproj	\
	$(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj \
	$(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj.filters

EXTRA_DIST += \
	$(top_builddir)/build/win32/vs9/test-interactive-clutter.vcproj	\
	$(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj \
	$(top_builddir)/build/win32/vs10/test-interactive-clutter.vcxproj.filters

clean-local: clean-wrappers
