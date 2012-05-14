/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Authored By Matthew Allum  <mallum@openedhand.com>
 *
 * Copyright (C) 2006, 2007, 2008 OpenedHand Ltd
 * Copyright (C) 2009, 2010, 2011, 2012 Intel Corp
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION:clutter-actor
 * @short_description: The basic element of the scene graph
 *
 * The ClutterActor class is the basic element of the scene graph in Clutter,
 * and it encapsulates the position, size, and transformations of a node in
 * the graph.
 *
 * <refsect2 id="ClutterActor-transformations">
 *   <title>Actor transformations</title>
 *   <para>Each actor can be transformed using methods like
 *   clutter_actor_set_scale() or clutter_actor_set_rotation(). The order
 *   in which the transformations are applied is decided by Clutter and it is
 *   the following:</para>
 *   <orderedlist>
 *     <listitem><para>translation by the origin of the #ClutterActor:allocation;</para></listitem>
 *     <listitem><para>translation by the actor's #ClutterActor:depth;</para></listitem>
 *     <listitem><para>scaling by the #ClutterActor:scale-x and #ClutterActor:scale-y factors;</para></listitem>
 *     <listitem><para>rotation around the #ClutterActor:rotation-z-angle and #ClutterActor:rotation-z-center;</para></listitem>
 *     <listitem><para>rotation around the #ClutterActor:rotation-y-angle and #ClutterActor:rotation-y-center;</para></listitem>
 *     <listitem><para>rotation around the #ClutterActor:rotation-x-angle and #ClutterActor:rotation-x-center;</para></listitem>
 *     <listitem><para>negative translation by the #ClutterActor:anchor-x and #ClutterActor:anchor-y point.</para></listitem>
 *   </orderedlist>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-geometry">
 *   <title>Modifying an actor's geometry</title>
 *   <para>Each actor has a bounding box, called #ClutterActor:allocation
 *   which is either set by its parent or explicitly through the
 *   clutter_actor_set_position() and clutter_actor_set_size() methods.
 *   Each actor also has an implicit preferred size.</para>
 *   <para>An actor’s preferred size can be defined by any subclass by
 *   overriding the #ClutterActorClass.get_preferred_width() and the
 *   #ClutterActorClass.get_preferred_height() virtual functions, or it can
 *   be explicitly set by using clutter_actor_set_width() and
 *   clutter_actor_set_height().</para>
 *   <para>An actor’s position can be set explicitly by using
 *   clutter_actor_set_x() and clutter_actor_set_y(); the coordinates are
 *   relative to the origin of the actor’s parent.</para>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-children">
 *   <title>Managing actor children</title>
 *   <para>Each actor can have multiple children, by calling
 *   clutter_actor_add_child() to add a new child actor, and
 *   clutter_actor_remove_child() to remove an existing child. #ClutterActor
 *   will hold a reference on each child actor, which will be released when
 *   the child is removed from its parent, or destroyed using
 *   clutter_actor_destroy().</para>
 *   <informalexample><programlisting>
 *  ClutterActor *actor = clutter_actor_new ();
 *
 *  /&ast; set the bounding box of the actor &ast;/
 *  clutter_actor_set_position (actor, 0, 0);
 *  clutter_actor_set_size (actor, 480, 640);
 *
 *  /&ast; set the background color of the actor &ast;/
 *  clutter_actor_set_background_color (actor, CLUTTER_COLOR_Orange);
 *
 *  /&ast; set the bounding box of the child, relative to the parent &ast;/
 *  ClutterActor *child = clutter_actor_new ();
 *  clutter_actor_set_position (child, 20, 20);
 *  clutter_actor_set_size (child, 80, 240);
 *
 *  /&ast; set the background color of the child &ast;/
 *  clutter_actor_set_background_color (child, CLUTTER_COLOR_Blue);
 *
 *  /&ast; add the child to the actor &ast;/
 *  clutter_actor_add_child (actor, child);
 *   </programlisting></informalexample>
 *   <para>Children can be inserted at a given index, or above and below
 *   another child actor. The order of insertion determines the order of the
 *   children when iterating over them. Iterating over children is performed
 *   by using clutter_actor_get_first_child(), clutter_actor_get_previous_sibling(),
 *   clutter_actor_get_next_sibling(), and clutter_actor_get_last_child(). It is
 *   also possible to retrieve a list of children by using
 *   clutter_actor_get_children(), as well as retrieving a specific child at a
 *   given index by using clutter_actor_get_child_at_index().</para>
 *   <para>If you need to track additions of children to a #ClutterActor, use
 *   the #ClutterContainer::actor-added signal; similarly, to track removals
 *   of children from a ClutterActor, use the #ClutterContainer::actor-removed
 *   signal.</para>
 *   <informalexample><programlisting>
 * <xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" href="../../../../tests/interactive/test-actor.c">
 *   <xi:fallback>FIXME: MISSING XINCLUDE CONTENT</xi:fallback>
 * </xi:include>
 *   </programlisting></informalexample>
 *   <figure id="actor-example-image">
 *     <title>Actors</title>
 *     <graphic fileref="actor-example.png" format="PNG"/>
 *   </figure>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-painting">
 *   <title>Painting an actor</title>
 *   <para>There are three ways to paint an actor:</para>
 *   <itemizedlist>
 *     <listitem><para>set a delegate #ClutterContent as the value for the
 *     #ClutterActor:content property of the actor;</para></listitem>
 *     <listitem><para>subclass #ClutterActor and override the
 *     #ClutterActorClass.paint_node() virtual function;</para></listitem>
 *     <listitem><para>subclass #ClutterActor and override the
 *     #ClutterActorClass.paint() virtual function.</para></listitem>
 *   </itemizedlist>
 *   <formalpara>
 *     <title>Setting the Content property</title>
 *     <para>A #ClutterContent is a delegate object that takes over the
 *     painting operation of one, or more actors. The #ClutterContent
 *     painting will be performed on top of the #ClutterActor:background-color
 *     of the actor, and before calling the #ClutterActorClass.paint_node()
 *     virtual function.</para>
 *     <informalexample><programlisting>
 * ClutterActor *actor = clutter_actor_new ();
 *
 * /&ast; set the bounding box &ast;/
 * clutter_actor_set_position (actor, 50, 50);
 * clutter_actor_set_size (actor, 100, 100);
 *
 * /&ast; set the content; the image_content variable is set elsewhere &ast;/
 * clutter_actor_set_content (actor, image_content);
 *     </programlisting></informalexample>
 *   </formalpara>
 *   <formalpara>
 *     <title>Overriding the paint_node virtual function</title>
 *     <para>The #ClutterActorClass.paint_node() virtual function is invoked
 *     whenever an actor needs to be painted. The implementation of the
 *     virtual function must only paint the contents of the actor itself,
 *     and not the contents of its children, if the actor has any.</para>
 *     <para>The #ClutterPaintNode passed to the virtual function is the
 *     local root of the render tree; any node added to it will be
 *     rendered at the correct position, as defined by the actor's
 *     #ClutterActor:allocation.</para>
 *     <informalexample><programlisting>
 * static void
 * my_actor_paint_node (ClutterActor     *actor,
 *                      ClutterPaintNode *root)
 * {
 *   ClutterPaintNode *node;
 *   ClutterActorBox box;
 *
 *   /&ast; where the content of the actor should be painted &ast;/
 *   clutter_actor_get_allocation_box (actor, &box);
 *
 *   /&ast; the cogl_texture variable is set elsewhere &ast;/
 *   node = clutter_texture_node_new (cogl_texture, CLUTTER_COLOR_White,
 *                                    CLUTTER_SCALING_FILTER_TRILINEAR,
 *                                    CLUTTER_SCALING_FILTER_LINEAR);
 *
 *   /&ast; paint the content of the node using the allocation &ast;/
 *   clutter_paint_node_add_rectangle (node, &box);
 *
 *   /&ast; add the node, and transfer ownership &ast;/
 *   clutter_paint_node_add_child (root, node);
 *   clutter_paint_node_unref (node);
 * }
 *     </programlisting></informalexample>
 *   </formalpara>
 *   <formalpara>
 *     <title>Overriding the paint virtual function</title>
 *     <para>The #ClutterActorClass.paint() virtual function is invoked
 *     when the #ClutterActor::paint signal is emitted, and after the other
 *     signal handlers have been invoked. Overriding the paint virtual
 *     function gives total control to the paint sequence of the actor
 *     itself, including the children of the actor, if any.</para>
 *     <warning><para>It is strongly discouraged to override the
 *     #ClutterActorClass.paint() virtual function, as well as connecting
 *     to the #ClutterActor::paint signal. These hooks into the paint
 *     sequence are considered legacy, and will be removed when the Clutter
 *     API changes.</para></warning>
 *   </formalpara>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-events">
 *   <title>Handling events on an actor</title>
 *   <para>A #ClutterActor can receive and handle input device events, for
 *   instance pointer events and key events, as long as its
 *   #ClutterActor:reactive property is set to %TRUE.</para>
 *   <para>Once an actor has been determined to be the source of an event,
 *   Clutter will traverse the scene graph from the top-level actor towards the
 *   event source, emitting the #ClutterActor::captured-event signal on each
 *   ancestor until it reaches the source; this phase is also called
 *   <emphasis>the capture phase</emphasis>. If the event propagation was not
 *   stopped, the graph is walked backwards, from the source actor to the
 *   top-level, and the #ClutterActor::event signal, along with other event
 *   signals if needed, is emitted; this phase is also called <emphasis>the
 *   bubble phase</emphasis>. At any point of the signal emission, signal
 *   handlers can stop the propagation through the scene graph by returning
 *   %CLUTTER_EVENT_STOP; otherwise, they can continue the propagation by
 *   returning %CLUTTER_EVENT_PROPAGATE.</para>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-animation">
 *   <title>Animation</title>
 *   <para>Animation is a core concept of modern user interfaces; Clutter
 *   provides a complete and powerful animation framework that automatically
 *   tweens the actor's state without requiring direct, frame by frame
 *   manipulation from your application code.</para>
 *   <formalpara>
 *     <title>Implicit animations</title>
 *     <para>The implicit animation model of Clutter assumes that all the
 *     changes in an actor state should be gradual and asynchronous; Clutter
 *     will automatically transition an actor's property change between the
 *     current state and the desired one without manual intervention.</para>
 *     <para>By default, in the 1.0 API series, the transition happens with
 *     a duration of zero milliseconds, and the implicit animation is an
 *     opt in feature to retain backwards compatibility. In order to enable
 *     implicit animations, it is necessary to change the easing state of
 *     an actor by using clutter_actor_save_easing_state():</para>
 *     <informalexample><programlisting>
 * /&ast; assume that the actor is currently positioned at (100, 100) &ast;/
 * clutter_actor_save_easing_state (actor);
 * clutter_actor_set_position (actor, 500, 500);
 * clutter_actor_restore_easing_state (actor);
 *     </programlisting></informalexample>
 *     <para>The example above will trigger an implicit animation of the
 *     actor between its current position to a new position.</para>
 *     <para>It is possible to animate multiple properties of an actor
 *     at the same time, and you can animate multiple actors at the same
 *     time as well, for instance:</para>
 *     <informalexample><programlisting>
 * /&ast; animate the actor's opacity and depth &ast;/
 * clutter_actor_save_easing_state (actor);
 * clutter_actor_set_opacity (actor, 0);
 * clutter_actor_set_depth (actor, -100);
 * clutter_actor_restore_easing_state (actor);
 *
 * /&ast; animate another actor's opacity &ast;/
 * clutter_actor_save_easing_state (another_actor);
 * clutter_actor_set_opacity (another_actor, 255);
 * clutter_actor_set_depth (another_actor, 100);
 * clutter_actor_restore_easing_state (another_actor);
 *     </programlisting></informalexample>
 *     <para>Implicit animations use a default duration of 250 milliseconds,
 *     and a default easing mode of %CLUTTER_EASE_OUT_CUBIC, unless you call
 *     clutter_actor_set_easing_mode() and clutter_actor_set_easing_duration()
 *     after changing the easing state of the actor.</para>
 *     <para>It is important to note that if you modify the state on an
 *     animatable property while a transition is in flight, the transition's
 *     final value will be updated, as well as its duration and progress
 *     mode by using the current easing state; for instance, in the following
 *     example:</para>
 *     <informalexample><programlisting>
 * clutter_actor_save_easing_state (actor);
 * clutter_actor_set_x (actor, 200);
 * clutter_actor_restore_easing_state (actor);
 *
 * clutter_actor_save_easing_state (actor);
 * clutter_actor_set_x (actor, 100);
 * clutter_actor_restore_easing_state (actor);
 *     </programlisting></informalexample>
 *     <para>the first call to clutter_actor_set_x() will begin a transition
 *     of the #ClutterActor:x property to the value of 200; the second call
 *     to clutter_actor_set_x() will change the transition's final value to
 *     100.</para>
 *     <para>It is possible to retrieve the #ClutterTransition used by the
 *     animatable properties by using clutter_actor_get_transition() and using
 *     the property name as the transition name.</para>
 *   </formalpara>
 *   <formalpara>
 *     <title>Explicit animations</title>
 *     <para>The explicit animation model supported by Clutter requires that
 *     you create a #ClutterTransition object, and set the initial and
 *     final values. The transition will not start unless you add it to the
 *     #ClutterActor.</para>
 *     <informalexample><programlisting>
 * ClutterTransition *transition;
 *
 * transition = clutter_property_transition_new ("opacity");
 * clutter_timeline_set_duration (CLUTTER_TIMELINE (transition), 3000);
 * clutter_timeline_set_repeat_count (CLUTTER_TIMELINE (transition), 2);
 * clutter_timeline_set_auto_reverse (CLUTTER_TIMELINE (transition), TRUE);
 * clutter_transition_set_interval (transition, clutter_interval_new (G_TYPE_UINT, 255, 0));
 *
 * clutter_actor_add_transition (actor, "animate-opacity", transition);
 *     </programlisting></informalexample>
 *     <para>The example above will animate the #ClutterActor:opacity property
 *     of an actor between fully opaque and fully transparent, and back, over
 *     a span of 3 seconds. The animation does not begin until it is added to
 *     the actor.</para>
 *     <para>The explicit animation API should also be used when using custom
 *     animatable properties for #ClutterAction, #ClutterConstraint, and
 *     #ClutterEffect instances associated to an actor; see the section on
 *     <ulink linkend="ClutterActor-custom-animatable-properties">custom
 *     animatable properties below</ulink> for an example.</para>
 *     <para>Finally, explicit animations are useful for creating animations
 *     that run continuously, for instance:</para>
 *     <informalexample><programlisting>
 * /&ast; this animation will pulse the actor's opacity continuously &ast;/
 * ClutterTransition *transition;
 * ClutterInterval *interval;
 *
 * transition = clutter_property_transition_new ("opacity");
 *
 * /&ast; we want to animate the opacity between 0 and 255 &ast;/
 * internal = clutter_interval_new (G_TYPE_UINT, 0, 255);
 * clutter_transition_set_interval (transition, interval);
 *
 * /&ast; over a one second duration, running an infinite amount of times &ast;/
 * clutter_timeline_set_duration (CLUTTER_TIMELINE (transition), 1000);
 * clutter_timeline_set_repeat_count (CLUTTER_TIMELINE (transition), -1);
 *
 * /&ast; we want to fade in and out, so we need to auto-reverse the transition &ast;/
 * clutter_timeline_set_auto_reverse (CLUTTER_TIMELINE (transition), TRUE);
 *
 * /&ast; and we want to use an easing function that eases both in and out &ast;/
 * clutter_timeline_set_progress_mode (CLUTTER_TIMELINE (transition),
 *                                     CLUTTER_EASE_IN_OUT_CUBIC);
 *
 * /&ast; add the transition to the desired actor; this will
 *  &ast; start the animation.
 *  &ast;/
 * clutter_actor_add_transition (actor, "opacityAnimation", transition);
 *     </programlisting></informalexample>
 *   </formalpara>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-subclassing">
 *   <title>Implementing an actor</title>
 *   <para>Careful consideration should be given when deciding to implement
 *   a #ClutterActor sub-class. It is generally recommended to implement a
 *   sub-class of #ClutterActor only for actors that should be used as leaf
 *   nodes of a scene graph.</para>
 *   <para>If your actor should be painted in a custom way, you should
 *   override the #ClutterActor::paint signal class handler. You can either
 *   opt to chain up to the parent class implementation or decide to fully
 *   override the default paint implementation; Clutter will set up the
 *   transformations and clip regions prior to emitting the #ClutterActor::paint
 *   signal.</para>
 *   <para>By overriding the #ClutterActorClass.get_preferred_width() and
 *   #ClutterActorClass.get_preferred_height() virtual functions it is
 *   possible to change or provide the preferred size of an actor; similarly,
 *   by overriding the #ClutterActorClass.allocate() virtual function it is
 *   possible to control the layout of the children of an actor. Make sure to
 *   always chain up to the parent implementation of the
 *   #ClutterActorClass.allocate() virtual function.</para>
 *   <para>In general, it is strongly encouraged to use delegation and
 *   composition instead of direct subclassing.</para>
 * </refsect2>
 *
 * <refsect2 id="ClutterActor-script">
 *   <title>ClutterActor custom properties for #ClutterScript</title>
 *   <para>#ClutterActor defines a custom "rotation" property which
 *   allows a short-hand description of the rotations to be applied
 *   to an actor.</para>
 *   <para>The syntax of the "rotation" property is the following:</para>
 *   <informalexample>
 *     <programlisting>
 * "rotation" : [
 *   { "&lt;axis&gt;" : [ &lt;angle&gt;, [ &lt;center&gt; ] ] }
 * ]
 *     </programlisting>
 *   </informalexample>
 *   <para>where the <emphasis>axis</emphasis> is the name of an enumeration
 *   value of type #ClutterRotateAxis and <emphasis>angle</emphasis> is a
 *   floating point value representing the rotation angle on the given axis,
 *   in degrees.</para>
 *   <para>The <emphasis>center</emphasis> array is optional, and if present
 *   it must contain the center of rotation as described by two coordinates:
 *   Y and Z for "x-axis"; X and Z for "y-axis"; and X and Y for
 *   "z-axis".</para>
 *   <para>#ClutterActor will also parse every positional and dimensional
 *   property defined as a string through clutter_units_from_string(); you
 *   should read the documentation for the #ClutterUnits parser format for
 *   the valid units and syntax.</para>
 * </refsect2>
 */

/**
 * CLUTTER_ACTOR_IS_MAPPED:
 * @a: a #ClutterActor
 *
 * Evaluates to %TRUE if the %CLUTTER_ACTOR_MAPPED flag is set.
 *
 * The mapped state is set when the actor is visible and all its parents up
 * to a top-level (e.g. a #ClutterStage) are visible, realized, and mapped.
 *
 * This check can be used to see if an actor is going to be painted, as only
 * actors with the %CLUTTER_ACTOR_MAPPED flag set are going to be painted.
 *
 * The %CLUTTER_ACTOR_MAPPED flag is managed by Clutter itself, and it should
 * not be checked directly; instead, the recommended usage is to connect a
 * handler on the #GObject::notify signal for the #ClutterActor:mapped
 * property of #ClutterActor, and check the presence of
 * the %CLUTTER_ACTOR_MAPPED flag on state changes.
 *
 * It is also important to note that Clutter may delay the changes of
 * the %CLUTTER_ACTOR_MAPPED flag on top-levels due to backend-specific
 * limitations, or during the reparenting of an actor, to optimize
 * unnecessary (and potentially expensive) state changes.
 *
 * Since: 0.2
 */

/**
 * CLUTTER_ACTOR_IS_REALIZED:
 * @a: a #ClutterActor
 *
 * Evaluates to %TRUE if the %CLUTTER_ACTOR_REALIZED flag is set.
 *
 * The realized state has an actor-dependant interpretation. If an
 * actor wants to delay allocating resources until it is attached to a
 * stage, it may use the realize state to do so. However it is
 * perfectly acceptable for an actor to allocate Cogl resources before
 * being realized because there is only one drawing context used by Clutter
 * so any resources will work on any stage.  If an actor is mapped it
 * must also be realized, but an actor can be realized and unmapped
 * (this is so hiding an actor temporarily doesn't do an expensive
 * unrealize/realize).
 *
 * To be realized an actor must be inside a stage, and all its parents
 * must be realized.
 *
 * Since: 0.2
 */

/**
 * CLUTTER_ACTOR_IS_VISIBLE:
 * @a: a #ClutterActor
 *
 * Evaluates to %TRUE if the actor has been shown, %FALSE if it's hidden.
 * Equivalent to the ClutterActor::visible object property.
 *
 * Note that an actor is only painted onscreen if it's mapped, which
 * means it's visible, and all its parents are visible, and one of the
 * parents is a toplevel stage; see also %CLUTTER_ACTOR_IS_MAPPED.
 *
 * Since: 0.2
 */

/**
 * CLUTTER_ACTOR_IS_REACTIVE:
 * @a: a #ClutterActor
 *
 * Evaluates to %TRUE if the %CLUTTER_ACTOR_REACTIVE flag is set.
 *
 * Only reactive actors will receive event-related signals.
 *
 * Since: 0.6
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include <gobject/gvaluecollector.h>

#include <cogl/cogl.h>

#define CLUTTER_ENABLE_EXPERIMENTAL_API

#include "clutter-actor-private.h"

#include "clutter-actor-meta-private.h"
#include "clutter-animatable.h"
#include "clutter-color-static.h"
#include "clutter-color.h"
#include "clutter-container.h"
#include "clutter-debug.h"
#include "clutter-effect-private.h"
#include "clutter-enum-types.h"
#include "clutter-flatten-effect.h"
#include "clutter-interval.h"
#include "clutter-main.h"
#include "clutter-marshal.h"
#include "clutter-paint-volume-private.h"
#include "clutter-private.h"
#include "clutter-profile.h"
#include "clutter-property-transition.h"
#include "clutter-scriptable.h"
#include "clutter-script-private.h"
#include "clutter-stage-private.h"
#include "clutter-timeline.h"
#include "clutter-transition.h"
#include "clutter-units.h"

#define CLUTTER_ACTOR_GET_PRIVATE(obj) \
(G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_ACTOR, ClutterActorPrivate))

#define ACTOR_GET_READ_DATA(actor) \
  ((const ClutterTransformInfo *) _clutter_actor_get_read_data (actor))
#define ACTOR_GET_WRITE_DATA(actor) \
  ((ClutterTransformInfo *) _clutter_actor_get_write_data (actor))

/* Internal enum used to control mapped state update.  This is a hint
 * which indicates when to do something other than just enforce
 * invariants.
 */
typedef enum {
  MAP_STATE_CHECK,           /* just enforce invariants. */
  MAP_STATE_MAKE_UNREALIZED, /* force unrealize, ignoring invariants,
                              * used when about to unparent.
                              */
  MAP_STATE_MAKE_MAPPED,     /* set mapped, error if invariants not met;
                              * used to set mapped on toplevels.
                              */
  MAP_STATE_MAKE_UNMAPPED    /* set unmapped, even if parent is mapped,
                              * used just before unmapping parent.
                              */
} MapStateChange;

struct _ClutterActorPrivate
{
  ClutterTransformInfo *info;

  /* clip, in actor coordinates */
  cairo_rectangle_t clip;

  /* the cached transformation matrix; see apply_transform() */
  CoglMatrix transform;

  guint8 opacity;
  gint opacity_override;

  ClutterOffscreenRedirect offscreen_redirect;

  /* This is an internal effect used to implement the
     offscreen-redirect property */
  ClutterEffect *flatten_effect;

  /* scene graph */
  ClutterActor *parent;
  ClutterActor *prev_sibling;
  ClutterActor *next_sibling;
  ClutterActor *first_child;
  ClutterActor *last_child;

  gint n_children;

  /* tracks whenever the children of an actor are changed; the
   * age is incremented by 1 whenever an actor is added or
   * removed. the age is not incremented when the first or the
   * last child pointers are changed, or when grandchildren of
   * an actor are changed.
   */
  gint age;

  gchar *name; /* a non-unique name, used for debugging */
  guint32 id; /* unique id, used for backward compatibility */

  gint32 pick_id; /* per-stage unique id, used for picking */

  /* a back-pointer to the Pango context that we can use
   * to create pre-configured PangoLayout
   */
  PangoContext *pango_context;

  /* the text direction configured for this child - either by
   * application code, or by the actor's parent
   */
  ClutterTextDirection text_direction;

  /* a counter used to toggle the CLUTTER_INTERNAL_CHILD flag */
  gint internal_child;

  /* meta classes */
  ClutterMetaGroup *effects;

  /* used when painting, to update the paint volume */
  ClutterEffect *current_effect;

  /* This is used to store an effect which needs to be redrawn. A
     redraw can be queued to start from a particular effect. This is
     used by parametrised effects that can cache an image of the
     actor. If a parameter of the effect changes then it only needs to
     redraw the cached image, not the actual actor. The pointer is
     only valid if is_dirty == TRUE. If the pointer is NULL then the
     whole actor is dirty. */
  ClutterEffect *effect_to_redraw;

  /* This is used when painting effects to implement the
     clutter_actor_continue_paint() function. It points to the node in
     the list of effects that is next in the chain */
  const GList *next_effect_to_paint;

  ClutterPaintVolume paint_volume;

  /* NB: This volume isn't relative to this actor, it is in eye
   * coordinates so that it can remain valid after the actor changes.
   */
  ClutterPaintVolume last_paint_volume;

  ClutterStageQueueRedrawEntry *queue_redraw_entry;

  /* bitfields */

  /* fixed position and sizes */
  guint position_set                : 1;
  guint show_on_set_parent          : 1;
  guint has_clip                    : 1;
  guint enable_model_view_transform : 1;
  guint enable_paint_unmapped       : 1;
  guint has_pointer                 : 1;
  guint propagated_one_redraw       : 1;
  guint paint_volume_valid          : 1;
  guint last_paint_volume_valid     : 1;
  guint in_clone_paint              : 1;
  guint transform_valid             : 1;
  /* This is TRUE if anything has queued a redraw since we were last
     painted. In this case effect_to_redraw will point to an effect
     the redraw was queued from or it will be NULL if the redraw was
     queued without an effect. */
  guint is_dirty                    : 1;
  guint bg_color_set                : 1;
};

enum
{
  PROP_0,

  PROP_NAME,

  PROP_CLIP,
  PROP_HAS_CLIP,

  PROP_OFFSCREEN_REDIRECT,

  PROP_VISIBLE,
  PROP_MAPPED,
  PROP_REALIZED,
  PROP_REACTIVE,

  PROP_ROTATION_CENTER_X,
  PROP_ROTATION_CENTER_Y,
  PROP_ROTATION_CENTER_Z,

  /* This property only makes sense for the z rotation because the
     others would depend on the actor having a size along the
     z-axis */
  PROP_ROTATION_CENTER_Z_GRAVITY,

  PROP_SCALE_CENTER_X,
  PROP_SCALE_CENTER_Y,
  PROP_SCALE_GRAVITY,

  PROP_ANCHOR_X,
  PROP_ANCHOR_Y,
  PROP_ANCHOR_GRAVITY,

  PROP_SHOW_ON_SET_PARENT,

  PROP_TEXT_DIRECTION,
  PROP_HAS_POINTER,

  PROP_EFFECT,

  PROP_BACKGROUND_COLOR_SET,

  PROP_FIRST_CHILD,
  PROP_LAST_CHILD,

  PROP_LAST
};

/* Animatable properties */
enum
{
  PROP_ANIMATABLE_0,

  PROP_OPACITY,

  PROP_X,
  PROP_Y,

  PROP_WIDTH,
  PROP_HEIGHT,

  PROP_DEPTH,

  PROP_SCALE_X,
  PROP_SCALE_Y,

  PROP_ROTATION_ANGLE_X,
  PROP_ROTATION_ANGLE_Y,
  PROP_ROTATION_ANGLE_Z,

  PROP_BACKGROUND_COLOR,

  PROP_ANIMATABLE_LAST
};

static GParamSpec *obj_props[PROP_LAST];
static GParamSpec *obj_anim_props[PROP_ANIMATABLE_LAST];

enum
{
  SHOW,
  HIDE,
  DESTROY,
  PARENT_SET,
  KEY_FOCUS_IN,
  KEY_FOCUS_OUT,
  PAINT,
  PICK,
  REALIZE,
  UNREALIZE,
  QUEUE_REDRAW,
  QUEUE_RELAYOUT,
  EVENT,
  CAPTURED_EVENT,
  BUTTON_PRESS_EVENT,
  BUTTON_RELEASE_EVENT,
  SCROLL_EVENT,
  KEY_PRESS_EVENT,
  KEY_RELEASE_EVENT,
  MOTION_EVENT,
  ENTER_EVENT,
  LEAVE_EVENT,
  TRANSITIONS_COMPLETED,

  LAST_SIGNAL
};

static guint actor_signals[LAST_SIGNAL] = { 0, };

static void clutter_container_iface_init  (ClutterContainerIface  *iface);
static void clutter_scriptable_iface_init (ClutterScriptableIface *iface);
static void clutter_animatable_iface_init (ClutterAnimatableIface *iface);
static void atk_implementor_iface_init    (AtkImplementorIface    *iface);

/* These setters are all static for now, maybe they should be in the
 * public API, but they are perhaps obscure enough to leave only as
 * properties
 */
static void clutter_actor_update_map_state       (ClutterActor  *self,
                                                  MapStateChange change);
static void clutter_actor_unrealize_not_hiding   (ClutterActor *self);

/* Helper routines for managing anchor coords */
static void clutter_anchor_coord_get_units (ClutterActor      *self,
                                            const AnchorCoord *coord,
                                            gfloat            *x,
                                            gfloat            *y,
                                            gfloat            *z);
static void clutter_anchor_coord_set_units (AnchorCoord       *coord,
                                            gfloat             x,
                                            gfloat             y,
                                            gfloat             z);

static ClutterGravity clutter_anchor_coord_get_gravity (const AnchorCoord *coord);
static void           clutter_anchor_coord_set_gravity (AnchorCoord       *coord,
                                                        ClutterGravity     gravity);

static gboolean clutter_anchor_coord_is_zero (const AnchorCoord *coord);

static void _clutter_actor_get_relative_transformation_matrix (ClutterActor *self,
                                                               ClutterActor *ancestor,
                                                               CoglMatrix *matrix);

static ClutterPaintVolume *_clutter_actor_get_paint_volume_mutable (ClutterActor *self);

static guint8   clutter_actor_get_paint_opacity_internal        (ClutterActor *self);

static inline void clutter_actor_set_background_color_internal (ClutterActor *self,
                                                                const ClutterColor *color);

/* Helper macro which translates by the anchor coord, applies the
   given transformation and then translates back */
#define TRANSFORM_ABOUT_ANCHOR_COORD(a,m,c,_transform)  G_STMT_START { \
  gfloat _tx, _ty, _tz;                                                \
  clutter_anchor_coord_get_units ((a), (c), &_tx, &_ty, &_tz);         \
  cogl_matrix_translate ((m), _tx, _ty, _tz);                          \
  { _transform; }                                                      \
  cogl_matrix_translate ((m), -_tx, -_ty, -_tz);        } G_STMT_END

static GQuark quark_shader_data = 0;
static GQuark quark_actor_transform_info = 0;
static GQuark quark_actor_animation_info = 0;

G_DEFINE_TYPE_WITH_CODE (ClutterActor,
                         clutter_actor,
                         G_TYPE_INITIALLY_UNOWNED,
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_CONTAINER,
                                                clutter_container_iface_init)
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_SCRIPTABLE,
                                                clutter_scriptable_iface_init)
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_ANIMATABLE,
                                                clutter_animatable_iface_init)
                         G_IMPLEMENT_INTERFACE (ATK_TYPE_IMPLEMENTOR,
                                                atk_implementor_iface_init));

/*< private >
 * clutter_actor_get_debug_name:
 * @actor: a #ClutterActor
 *
 * Retrieves a printable name of @actor for debugging messages
 *
 * Return value: a string with a printable name
 */
const gchar *
_clutter_actor_get_debug_name (ClutterActor *actor)
{
  return actor->priv->name != NULL ? actor->priv->name
                                   : G_OBJECT_TYPE_NAME (actor);
}

#ifdef CLUTTER_ENABLE_DEBUG
/* XXX - this is for debugging only, remove once working (or leave
 * in only in some debug mode). Should leave it for a little while
 * until we're confident in the new map/realize/visible handling.
 */
static inline void
clutter_actor_verify_map_state (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;

  if (CLUTTER_ACTOR_IS_REALIZED (self))
    {
      /* all bets are off during reparent when we're potentially realized,
       * but should not be according to invariants
       */
      if (!CLUTTER_ACTOR_IN_REPARENT (self))
        {
          if (priv->parent == NULL)
            {
              if (CLUTTER_ACTOR_IS_TOPLEVEL (self))
                {
                }
              else
                g_warning ("Realized non-toplevel actor '%s' should "
                           "have a parent",
                           _clutter_actor_get_debug_name (self));
            }
          else if (!CLUTTER_ACTOR_IS_REALIZED (priv->parent))
            {
              g_warning ("Realized actor %s has an unrealized parent %s",
                         _clutter_actor_get_debug_name (self),
                         _clutter_actor_get_debug_name (priv->parent));
            }
        }
    }

  if (CLUTTER_ACTOR_IS_MAPPED (self))
    {
      if (!CLUTTER_ACTOR_IS_REALIZED (self))
        g_warning ("Actor '%s' is mapped but not realized",
                   _clutter_actor_get_debug_name (self));

      /* remaining bets are off during reparent when we're potentially
       * mapped, but should not be according to invariants
       */
      if (!CLUTTER_ACTOR_IN_REPARENT (self))
        {
          if (priv->parent == NULL)
            {
              if (CLUTTER_ACTOR_IS_TOPLEVEL (self))
                {
                  if (!CLUTTER_ACTOR_IS_VISIBLE (self) &&
                      !CLUTTER_ACTOR_IN_DESTRUCTION (self))
                    {
                      g_warning ("Toplevel actor '%s' is mapped "
                                 "but not visible",
                                 _clutter_actor_get_debug_name (self));
                    }
                }
              else
                {
                  g_warning ("Mapped actor '%s' should have a parent",
                             _clutter_actor_get_debug_name (self));
                }
            }
          else
            {
              ClutterActor *iter = self;

              /* check for the enable_paint_unmapped flag on the actor
               * and parents; if the flag is enabled at any point of this
               * branch of the scene graph then all the later checks
               * become pointless
               */
              while (iter != NULL)
                {
                  if (iter->priv->enable_paint_unmapped)
                    return;

                  iter = iter->priv->parent;
                }

              if (!CLUTTER_ACTOR_IS_VISIBLE (priv->parent))
                {
                  g_warning ("Actor '%s' should not be mapped if parent '%s'"
                             "is not visible",
                             _clutter_actor_get_debug_name (self),
                             _clutter_actor_get_debug_name (priv->parent));
                }

              if (!CLUTTER_ACTOR_IS_REALIZED (priv->parent))
                {
                  g_warning ("Actor '%s' should not be mapped if parent '%s'"
                             "is not realized",
                             _clutter_actor_get_debug_name (self),
                             _clutter_actor_get_debug_name (priv->parent));
                }

              if (!CLUTTER_ACTOR_IS_TOPLEVEL (priv->parent))
                {
                  if (!CLUTTER_ACTOR_IS_MAPPED (priv->parent))
                    g_warning ("Actor '%s' is mapped but its non-toplevel "
                               "parent '%s' is not mapped",
                               _clutter_actor_get_debug_name (self),
                               _clutter_actor_get_debug_name (priv->parent));
                }
            }
        }
    }
}

#endif /* CLUTTER_ENABLE_DEBUG */

static void
clutter_actor_set_mapped (ClutterActor *self,
                          gboolean      mapped)
{
  if (CLUTTER_ACTOR_IS_MAPPED (self) == mapped)
    return;

  if (mapped)
    {
      CLUTTER_ACTOR_GET_CLASS (self)->map (self);
      g_assert (CLUTTER_ACTOR_IS_MAPPED (self));
    }
  else
    {
      CLUTTER_ACTOR_GET_CLASS (self)->unmap (self);
      g_assert (!CLUTTER_ACTOR_IS_MAPPED (self));
    }
}

/* this function updates the mapped and realized states according to
 * invariants, in the appropriate order.
 */
static void
clutter_actor_update_map_state (ClutterActor  *self,
                                MapStateChange change)
{
  gboolean was_mapped;

  was_mapped = CLUTTER_ACTOR_IS_MAPPED (self);

  if (CLUTTER_ACTOR_IS_TOPLEVEL (self))
    {
      /* the mapped flag on top-level actors must be set by the
       * per-backend implementation because it might be asynchronous.
       *
       * That is, the MAPPED flag on toplevels currently tracks the X
       * server mapped-ness of the window, while the expected behavior
       * (if used to GTK) may be to track WM_STATE!=WithdrawnState.
       * This creates some weird complexity by breaking the invariant
       * that if we're visible and all ancestors shown then we are
       * also mapped - instead, we are mapped if all ancestors
       * _possibly excepting_ the stage are mapped. The stage
       * will map/unmap for example when it is minimized or
       * moved to another workspace.
       *
       * So, the only invariant on the stage is that if visible it
       * should be realized, and that it has to be visible to be
       * mapped.
       */
      if (CLUTTER_ACTOR_IS_VISIBLE (self))
        clutter_actor_realize (self);

      switch (change)
        {
        case MAP_STATE_CHECK:
          break;

        case MAP_STATE_MAKE_MAPPED:
          g_assert (!was_mapped);
          clutter_actor_set_mapped (self, TRUE);
          break;

        case MAP_STATE_MAKE_UNMAPPED:
          g_assert (was_mapped);
          clutter_actor_set_mapped (self, FALSE);
          break;

        case MAP_STATE_MAKE_UNREALIZED:
          /* we only use MAKE_UNREALIZED in unparent,
           * and unparenting a stage isn't possible.
           * If someone wants to just unrealize a stage
           * then clutter_actor_unrealize() doesn't
           * go through this codepath.
           */
          g_warning ("Trying to force unrealize stage is not allowed");
          break;
        }

      if (CLUTTER_ACTOR_IS_MAPPED (self) &&
          !CLUTTER_ACTOR_IS_VISIBLE (self) &&
          !CLUTTER_ACTOR_IN_DESTRUCTION (self))
        {
          g_warning ("Clutter toplevel of type '%s' is not visible, but "
                     "it is somehow still mapped",
                     _clutter_actor_get_debug_name (self));
        }
    }
  else
    {
      ClutterActorPrivate *priv = self->priv;
      ClutterActor *parent = priv->parent;
      gboolean should_be_mapped;
      gboolean may_be_realized;
      gboolean must_be_realized;

      should_be_mapped = FALSE;
      may_be_realized = TRUE;
      must_be_realized = FALSE;

      if (parent == NULL || change == MAP_STATE_MAKE_UNREALIZED)
        {
          may_be_realized = FALSE;
        }
      else
        {
          /* Maintain invariant that if parent is mapped, and we are
           * visible, then we are mapped ...  unless parent is a
           * stage, in which case we map regardless of parent's map
           * state but do require stage to be visible and realized.
           *
           * If parent is realized, that does not force us to be
           * realized; but if parent is unrealized, that does force
           * us to be unrealized.
           *
           * The reason we don't force children to realize with
           * parents is _clutter_actor_rerealize(); if we require that
           * a realized parent means children are realized, then to
           * unrealize an actor we would have to unrealize its
           * parents, which would end up meaning unrealizing and
           * hiding the entire stage. So we allow unrealizing a
           * child (as long as that child is not mapped) while that
           * child still has a realized parent.
           *
           * Also, if we unrealize from leaf nodes to root, and
           * realize from root to leaf, the invariants are never
           * violated if we allow children to be unrealized
           * while parents are realized.
           *
           * When unmapping, MAP_STATE_MAKE_UNMAPPED is specified
           * to force us to unmap, even though parent is still
           * mapped. This is because we're unmapping from leaf nodes
           * up to root nodes.
           */
          if (CLUTTER_ACTOR_IS_VISIBLE (self) &&
              change != MAP_STATE_MAKE_UNMAPPED)
            {
              gboolean parent_is_visible_realized_toplevel;

              parent_is_visible_realized_toplevel =
                (CLUTTER_ACTOR_IS_TOPLEVEL (parent) &&
                 CLUTTER_ACTOR_IS_VISIBLE (parent) &&
                 CLUTTER_ACTOR_IS_REALIZED (parent));

              if (CLUTTER_ACTOR_IS_MAPPED (parent) ||
                  parent_is_visible_realized_toplevel)
                {
                  must_be_realized = TRUE;
                  should_be_mapped = TRUE;
                }
            }

          /* if the actor has been set to be painted even if unmapped
           * then we should map it and check for realization as well;
           * this is an override for the branch of the scene graph
           * which begins with this node
           */
          if (priv->enable_paint_unmapped)
            {
              if (priv->parent == NULL)
                g_warning ("Attempting to map an unparented actor '%s'",
                           _clutter_actor_get_debug_name (self));

              should_be_mapped = TRUE;
              must_be_realized = TRUE;
            }

          if (!CLUTTER_ACTOR_IS_REALIZED (parent))
            may_be_realized = FALSE;
        }

      if (change == MAP_STATE_MAKE_MAPPED && !should_be_mapped)
        {
          if (parent == NULL)
            g_warning ("Attempting to map a child that does not "
                       "meet the necessary invariants: the actor '%s' "
                       "has no parent",
                       _clutter_actor_get_debug_name (self));
          else
            g_warning ("Attempting to map a child that does not "
                       "meet the necessary invariants: the actor '%s' "
                       "is parented to an unmapped actor '%s'",
                       _clutter_actor_get_debug_name (self),
                       _clutter_actor_get_debug_name (priv->parent));
        }

      /* If in reparent, we temporarily suspend unmap and unrealize.
       *
       * We want to go in the order "realize, map" and "unmap, unrealize"
       */

      /* Unmap */
      if (!should_be_mapped && !CLUTTER_ACTOR_IN_REPARENT (self))
        clutter_actor_set_mapped (self, FALSE);

      /* Realize */
      if (must_be_realized)
        clutter_actor_realize (self);

      /* if we must be realized then we may be, presumably */
      g_assert (!(must_be_realized && !may_be_realized));

      /* Unrealize */
      if (!may_be_realized && !CLUTTER_ACTOR_IN_REPARENT (self))
        clutter_actor_unrealize_not_hiding (self);

      /* Map */
      if (should_be_mapped)
        {
          if (!must_be_realized)
            g_warning ("Somehow we think actor '%s' should be mapped but "
                       "not realized, which isn't allowed",
                       _clutter_actor_get_debug_name (self));

          /* realization is allowed to fail (though I don't know what
           * an app is supposed to do about that - shouldn't it just
           * be a g_error? anyway, we have to avoid mapping if this
           * happens)
           */
          if (CLUTTER_ACTOR_IS_REALIZED (self))
            clutter_actor_set_mapped (self, TRUE);
        }
    }

#ifdef CLUTTER_ENABLE_DEBUG
  /* check all invariants were kept */
  clutter_actor_verify_map_state (self);
#endif
}

static void
clutter_actor_real_map (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterActor *stage, *iter;

  g_assert (!CLUTTER_ACTOR_IS_MAPPED (self));

  CLUTTER_NOTE (ACTOR, "Mapping actor '%s'",
                _clutter_actor_get_debug_name (self));

  CLUTTER_ACTOR_SET_FLAGS (self, CLUTTER_ACTOR_MAPPED);

  stage = _clutter_actor_get_stage_internal (self);
  priv->pick_id = _clutter_stage_acquire_pick_id (CLUTTER_STAGE (stage), self);

  CLUTTER_NOTE (ACTOR, "Pick id '%d' for actor '%s'",
                priv->pick_id,
                _clutter_actor_get_debug_name (self));

  /* notify on parent mapped before potentially mapping
   * children, so apps see a top-down notification.
   */
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_MAPPED]);

  for (iter = self->priv->first_child;
       iter != NULL;
       iter = iter->priv->next_sibling)
    {
      clutter_actor_map (iter);
    }
}

/**
 * clutter_actor_map:
 * @self: A #ClutterActor
 *
 * Sets the %CLUTTER_ACTOR_MAPPED flag on the actor and possibly maps
 * and realizes its children if they are visible. Does nothing if the
 * actor is not visible.
 *
 * Calling this function is strongly disencouraged: the default
 * implementation of #ClutterActorClass.map() will map all the children
 * of an actor when mapping its parent.
 *
 * When overriding map, it is mandatory to chain up to the parent
 * implementation.
 *
 * Since: 1.0
 */
void
clutter_actor_map (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (CLUTTER_ACTOR_IS_MAPPED (self))
    return;

  if (!CLUTTER_ACTOR_IS_VISIBLE (self))
    return;

  clutter_actor_update_map_state (self, MAP_STATE_MAKE_MAPPED);
}

static void
clutter_actor_real_unmap (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterActor *iter;

  g_assert (CLUTTER_ACTOR_IS_MAPPED (self));

  CLUTTER_NOTE (ACTOR, "Unmapping actor '%s'",
                _clutter_actor_get_debug_name (self));

  for (iter = self->priv->first_child;
       iter != NULL;
       iter = iter->priv->next_sibling)
    {
      clutter_actor_unmap (iter);
    }

  CLUTTER_ACTOR_UNSET_FLAGS (self, CLUTTER_ACTOR_MAPPED);

  /* clear the contents of the last paint volume, so that hiding + moving +
   * showing will not result in the wrong area being repainted
   */
  _clutter_paint_volume_init_static (&priv->last_paint_volume, NULL);
  priv->last_paint_volume_valid = TRUE;

  /* notify on parent mapped after potentially unmapping
   * children, so apps see a bottom-up notification.
   */
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_MAPPED]);

  /* relinquish keyboard focus if we were unmapped while owning it */
  if (!CLUTTER_ACTOR_IS_TOPLEVEL (self))
    {
      ClutterStage *stage;

      stage = CLUTTER_STAGE (_clutter_actor_get_stage_internal (self));

      if (stage != NULL)
        _clutter_stage_release_pick_id (stage, priv->pick_id);

      priv->pick_id = -1;

      if (stage != NULL &&
          clutter_stage_get_key_focus (stage) == self)
        {
          clutter_stage_set_key_focus (stage, NULL);
        }
    }
}

/**
 * clutter_actor_unmap:
 * @self: A #ClutterActor
 *
 * Unsets the %CLUTTER_ACTOR_MAPPED flag on the actor and possibly
 * unmaps its children if they were mapped.
 *
 * Calling this function is not encouraged: the default #ClutterActor
 * implementation of #ClutterActorClass.unmap() will also unmap any
 * eventual children by default when their parent is unmapped.
 *
 * When overriding #ClutterActorClass.unmap(), it is mandatory to
 * chain up to the parent implementation.
 *
 * <note>It is important to note that the implementation of the
 * #ClutterActorClass.unmap() virtual function may be called after
 * the #ClutterActorClass.destroy() or the #GObjectClass.dispose()
 * implementation, but it is guaranteed to be called before the
 * #GObjectClass.finalize() implementation.</note>
 *
 * Since: 1.0
 */
void
clutter_actor_unmap (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (!CLUTTER_ACTOR_IS_MAPPED (self))
    return;

  clutter_actor_update_map_state (self, MAP_STATE_MAKE_UNMAPPED);
}

static void
clutter_actor_real_show (ClutterActor *self)
{
  if (!CLUTTER_ACTOR_IS_VISIBLE (self))
    {
      CLUTTER_ACTOR_SET_FLAGS (self, CLUTTER_ACTOR_VISIBLE);

      /* we notify on the "visible" flag in the clutter_actor_show()
       * wrapper so the entire show signal emission completes first
       * (?)
       */
      clutter_actor_update_map_state (self, MAP_STATE_CHECK);
    }
}

static inline void
set_show_on_set_parent (ClutterActor *self,
                        gboolean      set_show)
{
  ClutterActorPrivate *priv = self->priv;

  set_show = !!set_show;

  if (priv->show_on_set_parent == set_show)
    return;

  if (priv->parent == NULL)
    {
      priv->show_on_set_parent = set_show;
      g_object_notify_by_pspec (G_OBJECT (self),
                                obj_props[PROP_SHOW_ON_SET_PARENT]);
    }
}

/**
 * clutter_actor_show:
 * @self: A #ClutterActor
 *
 * Flags an actor to be displayed. An actor that isn't shown will not
 * be rendered on the stage.
 *
 * Actors are visible by default.
 *
 * If this function is called on an actor without a parent, the
 * #ClutterActor:show-on-set-parent will be set to %TRUE as a side
 * effect.
 */
void
clutter_actor_show (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  /* simple optimization */
  if (CLUTTER_ACTOR_IS_VISIBLE (self))
    {
      /* we still need to set the :show-on-set-parent property, in
       * case show() is called on an unparented actor
       */
      set_show_on_set_parent (self, TRUE);
      return;
    }

#ifdef CLUTTER_ENABLE_DEBUG
  clutter_actor_verify_map_state (self);
#endif

  priv = self->priv;

  g_object_freeze_notify (G_OBJECT (self));

  set_show_on_set_parent (self, TRUE);

  g_signal_emit (self, actor_signals[SHOW], 0);
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_VISIBLE]);

  if (priv->parent != NULL)
    clutter_actor_queue_redraw (priv->parent);

  g_object_thaw_notify (G_OBJECT (self));
}

static void
clutter_actor_real_hide (ClutterActor *self)
{
  if (CLUTTER_ACTOR_IS_VISIBLE (self))
    {
      CLUTTER_ACTOR_UNSET_FLAGS (self, CLUTTER_ACTOR_VISIBLE);

      /* we notify on the "visible" flag in the clutter_actor_hide()
       * wrapper so the entire hide signal emission completes first
       * (?)
       */
      clutter_actor_update_map_state (self, MAP_STATE_CHECK);
    }
}

/**
 * clutter_actor_hide:
 * @self: A #ClutterActor
 *
 * Flags an actor to be hidden. A hidden actor will not be
 * rendered on the stage.
 *
 * Actors are visible by default.
 *
 * If this function is called on an actor without a parent, the
 * #ClutterActor:show-on-set-parent property will be set to %FALSE
 * as a side-effect.
 */
void
clutter_actor_hide (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  /* simple optimization */
  if (!CLUTTER_ACTOR_IS_VISIBLE (self))
    {
      /* we still need to set the :show-on-set-parent property, in
       * case hide() is called on an unparented actor
       */
      set_show_on_set_parent (self, FALSE);
      return;
    }

#ifdef CLUTTER_ENABLE_DEBUG
  clutter_actor_verify_map_state (self);
#endif

  priv = self->priv;

  g_object_freeze_notify (G_OBJECT (self));

  set_show_on_set_parent (self, FALSE);

  g_signal_emit (self, actor_signals[HIDE], 0);
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_VISIBLE]);

  if (priv->parent != NULL)
    clutter_actor_queue_redraw (priv->parent);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_realize:
 * @self: A #ClutterActor
 *
 * Realization informs the actor that it is attached to a stage. It
 * can use this to allocate resources if it wanted to delay allocation
 * until it would be rendered. However it is perfectly acceptable for
 * an actor to create resources before being realized because Clutter
 * only ever has a single rendering context so that actor is free to
 * be moved from one stage to another.
 *
 * This function does nothing if the actor is already realized.
 *
 * Because a realized actor must have realized parent actors, calling
 * clutter_actor_realize() will also realize all parents of the actor.
 *
 * This function does not realize child actors, except in the special
 * case that realizing the stage, when the stage is visible, will
 * suddenly map (and thus realize) the children of the stage.
 **/
void
clutter_actor_realize (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  priv = self->priv;

#ifdef CLUTTER_ENABLE_DEBUG
  clutter_actor_verify_map_state (self);
#endif

  if (CLUTTER_ACTOR_IS_REALIZED (self))
    return;

  /* To be realized, our parent actors must be realized first.
   * This will only succeed if we're inside a toplevel.
   */
  if (priv->parent != NULL)
    clutter_actor_realize (priv->parent);

  if (CLUTTER_ACTOR_IS_TOPLEVEL (self))
    {
      /* toplevels can be realized at any time */
    }
  else
    {
      /* "Fail" the realization if parent is missing or unrealized;
       * this should really be a g_warning() not some kind of runtime
       * failure; how can an app possibly recover? Instead it's a bug
       * in the app and the app should get an explanatory warning so
       * someone can fix it. But for now it's too hard to fix this
       * because e.g. ClutterTexture needs reworking.
       */
      if (priv->parent == NULL ||
          !CLUTTER_ACTOR_IS_REALIZED (priv->parent))
        return;
    }

  CLUTTER_NOTE (ACTOR, "Realizing actor '%s'", _clutter_actor_get_debug_name (self));

  CLUTTER_ACTOR_SET_FLAGS (self, CLUTTER_ACTOR_REALIZED);
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_REALIZED]);

  g_signal_emit (self, actor_signals[REALIZE], 0);

  /* Stage actor is allowed to unset the realized flag again in its
   * default signal handler, though that is a pathological situation.
   */

  /* If realization "failed" we'll have to update child state. */
  clutter_actor_update_map_state (self, MAP_STATE_CHECK);
}

static void
clutter_actor_real_unrealize (ClutterActor *self)
{
  /* we must be unmapped (implying our children are also unmapped) */
  g_assert (!CLUTTER_ACTOR_IS_MAPPED (self));
}

/**
 * clutter_actor_unrealize:
 * @self: A #ClutterActor
 *
 * Unrealization informs the actor that it may be being destroyed or
 * moved to another stage. The actor may want to destroy any
 * underlying graphics resources at this point. However it is
 * perfectly acceptable for it to retain the resources until the actor
 * is destroyed because Clutter only ever uses a single rendering
 * context and all of the graphics resources are valid on any stage.
 *
 * Because mapped actors must be realized, actors may not be
 * unrealized if they are mapped. This function hides the actor to be
 * sure it isn't mapped, an application-visible side effect that you
 * may not be expecting.
 *
 * This function should not be called by application code.
 */
void
clutter_actor_unrealize (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (!CLUTTER_ACTOR_IS_MAPPED (self));

/* This function should not really be in the public API, because
 * there isn't a good reason to call it. ClutterActor will already
 * unrealize things for you when it's important to do so.
 *
 * If you were using clutter_actor_unrealize() in a dispose
 * implementation, then don't, just chain up to ClutterActor's
 * dispose.
 *
 * If you were using clutter_actor_unrealize() to implement
 * unrealizing children of your container, then don't, ClutterActor
 * will already take care of that.
 *
 * If you were using clutter_actor_unrealize() to re-realize to
 * create your resources in a different way, then use
 * _clutter_actor_rerealize() (inside Clutter) or just call your
 * code that recreates your resources directly (outside Clutter).
 */

#ifdef CLUTTER_ENABLE_DEBUG
  clutter_actor_verify_map_state (self);
#endif

  clutter_actor_hide (self);

  clutter_actor_unrealize_not_hiding (self);
}

static ClutterActorTraverseVisitFlags
unrealize_actor_before_children_cb (ClutterActor *self,
                                    int depth,
                                    void *user_data)
{
  /* If an actor is already unrealized we know its children have also
   * already been unrealized... */
  if (!CLUTTER_ACTOR_IS_REALIZED (self))
    return CLUTTER_ACTOR_TRAVERSE_VISIT_SKIP_CHILDREN;

  g_signal_emit (self, actor_signals[UNREALIZE], 0);

  return CLUTTER_ACTOR_TRAVERSE_VISIT_CONTINUE;
}

static ClutterActorTraverseVisitFlags
unrealize_actor_after_children_cb (ClutterActor *self,
                                   int depth,
                                   void *user_data)
{
  /* We want to unset the realized flag only _after_
   * child actors are unrealized, to maintain invariants.
   */
  CLUTTER_ACTOR_UNSET_FLAGS (self, CLUTTER_ACTOR_REALIZED);
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_REALIZED]);
  return CLUTTER_ACTOR_TRAVERSE_VISIT_CONTINUE;
}

/*
 * clutter_actor_unrealize_not_hiding:
 * @self: A #ClutterActor
 *
 * Unrealization informs the actor that it may be being destroyed or
 * moved to another stage. The actor may want to destroy any
 * underlying graphics resources at this point. However it is
 * perfectly acceptable for it to retain the resources until the actor
 * is destroyed because Clutter only ever uses a single rendering
 * context and all of the graphics resources are valid on any stage.
 *
 * Because mapped actors must be realized, actors may not be
 * unrealized if they are mapped. You must hide the actor or one of
 * its parents before attempting to unrealize.
 *
 * This function is separate from clutter_actor_unrealize() because it
 * does not automatically hide the actor.
 * Actors need not be hidden to be unrealized, they just need to
 * be unmapped. In fact we don't want to mess up the application's
 * setting of the "visible" flag, so hiding is very undesirable.
 *
 * clutter_actor_unrealize() does a clutter_actor_hide() just for
 * backward compatibility.
 */
static void
clutter_actor_unrealize_not_hiding (ClutterActor *self)
{
  _clutter_actor_traverse (self,
                           CLUTTER_ACTOR_TRAVERSE_DEPTH_FIRST,
                           unrealize_actor_before_children_cb,
                           unrealize_actor_after_children_cb,
                           NULL);
}

/*
 * _clutter_actor_rerealize:
 * @self: A #ClutterActor
 * @callback: Function to call while unrealized
 * @data: data for callback
 *
 * If an actor is already unrealized, this just calls the callback.
 *
 * If it is realized, it unrealizes temporarily, calls the callback,
 * and then re-realizes the actor.
 *
 * As a side effect, leaves all children of the actor unrealized if
 * the actor was realized but not showing.  This is because when we
 * unrealize the actor temporarily we must unrealize its children
 * (e.g. children of a stage can't be realized if stage window is
 * gone). And we aren't clever enough to save the realization state of
 * all children. In most cases this should not matter, because
 * the children will automatically realize when they next become mapped.
 */
void
_clutter_actor_rerealize (ClutterActor    *self,
                          ClutterCallback  callback,
                          void            *data)
{
  gboolean was_mapped;
  gboolean was_showing;
  gboolean was_realized;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

#ifdef CLUTTER_ENABLE_DEBUG
  clutter_actor_verify_map_state (self);
#endif

  was_realized = CLUTTER_ACTOR_IS_REALIZED (self);
  was_mapped = CLUTTER_ACTOR_IS_MAPPED (self);
  was_showing = CLUTTER_ACTOR_IS_VISIBLE (self);

  /* Must be unmapped to unrealize. Note we only have to hide this
   * actor if it was mapped (if all parents were showing).  If actor
   * is merely visible (but not mapped), then that's fine, we can
   * leave it visible.
   */
  if (was_mapped)
    clutter_actor_hide (self);

  g_assert (!CLUTTER_ACTOR_IS_MAPPED (self));

  /* unrealize self and all children */
  clutter_actor_unrealize_not_hiding (self);

  if (callback != NULL)
    {
      (* callback) (self, data);
    }

  if (was_showing)
    clutter_actor_show (self); /* will realize only if mapping implies it */
  else if (was_realized)
    clutter_actor_realize (self); /* realize self and all parents */
}

static void
clutter_actor_real_pick (ClutterActor       *self,
			 const ClutterColor *color)
{
  const ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info_or_defaults (self);

  /* the default implementation is just to paint a rectangle
   * with the same size of the actor using the passed color
   */
  if (clutter_actor_should_pick_paint (self))
    {
      cogl_set_source_color4ub (color->red,
                                color->green,
                                color->blue,
                                color->alpha);

      cogl_rectangle (0, 0, info->width, info->height);
    }

  /* XXX - this thoroughly sucks, but we need to maintain compatibility
   * with existing container classes that override the pick() virtual
   * and chain up to the default implementation - otherwise we'll end up
   * painting our children twice.
   *
   * this has to go away for 2.0; hopefully along the pick() itself.
   */
  if (CLUTTER_ACTOR_GET_CLASS (self)->pick == clutter_actor_real_pick)
    {
      ClutterActor *iter;

      for (iter = self->priv->first_child;
           iter != NULL;
           iter = iter->priv->next_sibling)
        clutter_actor_paint (iter);
    }
}

/**
 * clutter_actor_should_pick_paint:
 * @self: A #ClutterActor
 *
 * Should be called inside the implementation of the
 * #ClutterActor::pick virtual function in order to check whether
 * the actor should paint itself in pick mode or not.
 *
 * This function should never be called directly by applications.
 *
 * Return value: %TRUE if the actor should paint its silhouette,
 *   %FALSE otherwise
 */
gboolean
clutter_actor_should_pick_paint (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  if (CLUTTER_ACTOR_IS_MAPPED (self) &&
      (_clutter_context_get_pick_mode () == CLUTTER_PICK_ALL ||
       CLUTTER_ACTOR_IS_REACTIVE (self)))
    return TRUE;

  return FALSE;
}

static void
clutter_actor_store_old_geometry (ClutterActor    *self,
                                  ClutterActorBox *box)
{
  const ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info_or_defaults (self);

  box->x1 = info->fixed_x;
  box->y1 = info->fixed_y;
  box->x2 = box->x1 + info->width;
  box->y1 = box->y1 + info->height;
}

static void
_clutter_actor_signal_queue_redraw (ClutterActor *self,
                                    ClutterActor *origin)
{
  /* no point in queuing a redraw on a destroyed actor */
  if (CLUTTER_ACTOR_IN_DESTRUCTION (self))
    return;

  /* NB: We can't bail out early here if the actor is hidden in case
   * the actor bas been cloned. In this case the clone will need to
   * receive the signal so it can queue its own redraw.
   */

  /* calls klass->queue_redraw in default handler */
  g_signal_emit (self, actor_signals[QUEUE_REDRAW], 0, origin);
}

static void
clutter_actor_real_queue_redraw (ClutterActor *self,
                                 ClutterActor *origin)
{
  ClutterActor *parent;

  CLUTTER_NOTE (PAINT, "Redraw queued on '%s' (from: '%s')",
                _clutter_actor_get_debug_name (self),
                origin != NULL ? _clutter_actor_get_debug_name (origin)
                               : "same actor");

  /* no point in queuing a redraw on a destroyed actor */
  if (CLUTTER_ACTOR_IN_DESTRUCTION (self))
    return;

  /* If the queue redraw is coming from a child then the actor has
     become dirty and any queued effect is no longer valid */
  if (self != origin)
    {
      self->priv->is_dirty = TRUE;
      self->priv->effect_to_redraw = NULL;
    }

  /* If the actor isn't visible, we still had to emit the signal
   * to allow for a ClutterClone, but the appearance of the parent
   * won't change so we don't have to propagate up the hierarchy.
   */
  if (!CLUTTER_ACTOR_IS_VISIBLE (self))
    return;

  /* Although we could determine here that a full stage redraw
   * has already been queued and immediately bail out, we actually
   * guarantee that we will propagate a queue-redraw signal to our
   * parent at least once so that it's possible to implement a
   * container that tracks which of its children have queued a
   * redraw.
   */
  if (self->priv->propagated_one_redraw)
    {
      ClutterActor *stage = _clutter_actor_get_stage_internal (self);
      if (stage != NULL &&
          _clutter_stage_has_full_redraw_queued (CLUTTER_STAGE (stage)))
        return;
    }

  self->priv->propagated_one_redraw = TRUE;

  /* notify parents, if they are all visible eventually we'll
   * queue redraw on the stage, which queues the redraw idle.
   */
  parent = clutter_actor_get_parent (self);
  if (parent != NULL)
    {
      /* this will go up recursively */
      _clutter_actor_signal_queue_redraw (parent, origin);
    }
}

/**
 * clutter_actor_apply_relative_transform_to_point:
 * @self: A #ClutterActor
 * @ancestor: (allow-none): A #ClutterActor ancestor, or %NULL to use the
 *   default #ClutterStage
 * @point: A point as #ClutterVertex
 * @vertex: (out caller-allocates): The translated #ClutterVertex
 *
 * Transforms @point in coordinates relative to the actor into
 * ancestor-relative coordinates using the relevant transform
 * stack (i.e. scale, rotation, etc).
 *
 * If @ancestor is %NULL the ancestor will be the #ClutterStage. In
 * this case, the coordinates returned will be the coordinates on
 * the stage before the projection is applied. This is different from
 * the behaviour of clutter_actor_apply_transform_to_point().
 *
 * Since: 0.6
 */
void
clutter_actor_apply_relative_transform_to_point (ClutterActor        *self,
						 ClutterActor        *ancestor,
						 const ClutterVertex *point,
						 ClutterVertex       *vertex)
{
  gfloat w;
  CoglMatrix matrix;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (ancestor == NULL || CLUTTER_IS_ACTOR (ancestor));
  g_return_if_fail (point != NULL);
  g_return_if_fail (vertex != NULL);

  *vertex = *point;
  w = 1.0;

  if (ancestor == NULL)
    ancestor = _clutter_actor_get_stage_internal (self);

  if (ancestor == NULL)
    {
      *vertex = *point;
      return;
    }

  _clutter_actor_get_relative_transformation_matrix (self, ancestor, &matrix);
  cogl_matrix_transform_point (&matrix, &vertex->x, &vertex->y, &vertex->z, &w);
}

static gboolean
_clutter_actor_fully_transform_vertices (ClutterActor *self,
                                         const ClutterVertex *vertices_in,
                                         ClutterVertex *vertices_out,
                                         int n_vertices)
{
  ClutterActor *stage;
  CoglMatrix modelview;
  CoglMatrix projection;
  float viewport[4];

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  stage = _clutter_actor_get_stage_internal (self);

  /* We really can't do anything meaningful in this case so don't try
   * to do any transform */
  if (stage == NULL)
    return FALSE;

  /* Note: we pass NULL as the ancestor because we don't just want the modelview
   * that gets us to stage coordinates, we want to go all the way to eye
   * coordinates */
  _clutter_actor_apply_relative_transformation_matrix (self, NULL, &modelview);

  /* Fetch the projection and viewport */
  _clutter_stage_get_projection_matrix (CLUTTER_STAGE (stage), &projection);
  _clutter_stage_get_viewport (CLUTTER_STAGE (stage),
                               &viewport[0],
                               &viewport[1],
                               &viewport[2],
                               &viewport[3]);

  _clutter_util_fully_transform_vertices (&modelview,
                                          &projection,
                                          viewport,
                                          vertices_in,
                                          vertices_out,
                                          n_vertices);

  return TRUE;
}

/**
 * clutter_actor_apply_transform_to_point:
 * @self: A #ClutterActor
 * @point: A point as #ClutterVertex
 * @vertex: (out caller-allocates): The translated #ClutterVertex
 *
 * Transforms @point in coordinates relative to the actor
 * into screen-relative coordinates with the current actor
 * transformation (i.e. scale, rotation, etc)
 *
 * Since: 0.4
 **/
void
clutter_actor_apply_transform_to_point (ClutterActor        *self,
                                        const ClutterVertex *point,
                                        ClutterVertex       *vertex)
{
  g_return_if_fail (point != NULL);
  g_return_if_fail (vertex != NULL);
  _clutter_actor_fully_transform_vertices (self, point, vertex, 1);
}

/*
 * _clutter_actor_get_relative_transformation_matrix:
 * @self: The actor whose coordinate space you want to transform from.
 * @ancestor: The ancestor actor whose coordinate space you want to transform too
 *            or %NULL if you want to transform all the way to eye coordinates.
 * @matrix: A #CoglMatrix to store the transformation
 *
 * This gets a transformation @matrix that will transform coordinates from the
 * coordinate space of @self into the coordinate space of @ancestor.
 *
 * For example if you need a matrix that can transform the local actor
 * coordinates of @self into stage coordinates you would pass the actor's stage
 * pointer as the @ancestor.
 *
 * If you pass %NULL then the transformation will take you all the way through
 * to eye coordinates. This can be useful if you want to extract the entire
 * modelview transform that Clutter applies before applying the projection
 * transformation. If you want to explicitly set a modelview on a CoglFramebuffer
 * using cogl_set_modelview_matrix() for example then you would want a matrix
 * that transforms into eye coordinates.
 *
 * <note><para>This function explicitly initializes the given @matrix. If you just
 * want clutter to multiply a relative transformation with an existing matrix
 * you can use clutter_actor_apply_relative_transformation_matrix()
 * instead.</para></note>
 *
 */
/* XXX: We should consider caching the stage relative modelview along with
 * the actor itself */
static void
_clutter_actor_get_relative_transformation_matrix (ClutterActor *self,
                                                   ClutterActor *ancestor,
                                                   CoglMatrix *matrix)
{
  cogl_matrix_init_identity (matrix);

  _clutter_actor_apply_relative_transformation_matrix (self, ancestor, matrix);
}

/* Project the given @box into stage window coordinates, writing the
 * transformed vertices to @verts[]. */
static gboolean
_clutter_actor_transform_and_project_box (ClutterActor          *self,
					  const ClutterActorBox *box,
					  ClutterVertex          verts[])
{
  ClutterVertex box_vertices[4];

  box_vertices[0].x = box->x1;
  box_vertices[0].y = box->y1;
  box_vertices[0].z = 0;
  box_vertices[1].x = box->x2;
  box_vertices[1].y = box->y1;
  box_vertices[1].z = 0;
  box_vertices[2].x = box->x1;
  box_vertices[2].y = box->y2;
  box_vertices[2].z = 0;
  box_vertices[3].x = box->x2;
  box_vertices[3].y = box->y2;
  box_vertices[3].z = 0;

  return
    _clutter_actor_fully_transform_vertices (self, box_vertices, verts, 4);
}

/**
 * clutter_actor_get_allocation_vertices:
 * @self: A #ClutterActor
 * @ancestor: (allow-none): A #ClutterActor to calculate the vertices
 *   against, or %NULL to use the #ClutterStage
 * @verts: (out) (array fixed-size=4) (element-type Clutter.Vertex): return
 *   location for an array of 4 #ClutterVertex in which to store the result
 *
 * Calculates the transformed coordinates of the four corners of the
 * actor in the plane of @ancestor. The returned vertices relate to
 * the #ClutterActorBox coordinates as follows:
 * <itemizedlist>
 *   <listitem><para>@verts[0] contains (x1, y1)</para></listitem>
 *   <listitem><para>@verts[1] contains (x2, y1)</para></listitem>
 *   <listitem><para>@verts[2] contains (x1, y2)</para></listitem>
 *   <listitem><para>@verts[3] contains (x2, y2)</para></listitem>
 * </itemizedlist>
 *
 * If @ancestor is %NULL the ancestor will be the #ClutterStage. In
 * this case, the coordinates returned will be the coordinates on
 * the stage before the projection is applied. This is different from
 * the behaviour of clutter_actor_get_abs_allocation_vertices().
 *
 * Since: 0.6
 */
void
clutter_actor_get_allocation_vertices (ClutterActor  *self,
                                       ClutterActor  *ancestor,
                                       ClutterVertex  verts[])
{
  ClutterActorPrivate *priv;
  ClutterActorBox box;
  ClutterVertex vertices[4];
  CoglMatrix modelview;
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (ancestor == NULL || CLUTTER_IS_ACTOR (ancestor));

  if (ancestor == NULL)
    ancestor = _clutter_actor_get_stage_internal (self);

  /* Fallback to a NOP transform if the actor isn't parented under a
   * stage. */
  if (ancestor == NULL)
    ancestor = self;

  info = _clutter_actor_get_transform_info_or_defaults (self);

  vertices[0].x = 0;
  vertices[0].y = 0;
  vertices[0].z = 0;
  vertices[1].x = info->width;
  vertices[1].y = 0;
  vertices[1].z = 0;
  vertices[2].x = 0;
  vertices[2].y = info->height;
  vertices[2].z = 0;
  vertices[3].x = info->width;
  vertices[3].y = info->height;
  vertices[3].z = 0;

  _clutter_actor_get_relative_transformation_matrix (self, ancestor,
                                                     &modelview);

  cogl_matrix_transform_points (&modelview,
                                3,
                                sizeof (ClutterVertex),
                                vertices,
                                sizeof (ClutterVertex),
                                vertices,
                                4);
}

/**
 * clutter_actor_get_abs_allocation_vertices:
 * @self: A #ClutterActor
 * @verts: (out) (array fixed-size=4): Pointer to a location of an array
 *   of 4 #ClutterVertex where to store the result.
 *
 * Calculates the transformed screen coordinates of the four corners of
 * the actor; the returned vertices relate to the #ClutterActorBox
 * coordinates  as follows:
 * <itemizedlist>
 *   <listitem><para>v[0] contains (x1, y1)</para></listitem>
 *   <listitem><para>v[1] contains (x2, y1)</para></listitem>
 *   <listitem><para>v[2] contains (x1, y2)</para></listitem>
 *   <listitem><para>v[3] contains (x2, y2)</para></listitem>
 * </itemizedlist>
 *
 * Since: 0.4
 */
void
clutter_actor_get_abs_allocation_vertices (ClutterActor  *self,
                                           ClutterVertex  verts[])
{
  ClutterActorBox actor_space_allocation;
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_transform_info_or_defaults (self);

  /* NB: _clutter_actor_transform_and_project_box expects a box in the actor's
   * own coordinate space... */
  actor_space_allocation.x1 = 0;
  actor_space_allocation.y1 = 0;
  actor_space_allocation.x2 = info->width;
  actor_space_allocation.y2 = info->height;
  _clutter_actor_transform_and_project_box (self,
                                           &actor_space_allocation,
                                           verts);
}

static void
clutter_actor_real_apply_transform (ClutterActor *self,
                                    CoglMatrix   *matrix)
{
  ClutterActorPrivate *priv = self->priv;

  if (!priv->transform_valid)
    {
      CoglMatrix *transform = &priv->transform;
      const ClutterTransformInfo *info;

      info = _clutter_actor_get_transform_info_or_defaults (self);

      cogl_matrix_init_identity (transform);

      cogl_matrix_translate (transform,
                             info->fixed_x,
                             info->fixed_y,
                             0.0);

      if (info->depth)
        cogl_matrix_translate (transform, 0, 0, info->depth);

      /*
       * because the rotation involves translations, we must scale
       * before applying the rotations (if we apply the scale after
       * the rotations, the translations included in the rotation are
       * not scaled and so the entire object will move on the screen
       * as a result of rotating it).
       */
      if (info->scale_x != 1.0 || info->scale_y != 1.0)
        {
          TRANSFORM_ABOUT_ANCHOR_COORD (self, transform,
                                        &info->scale_center,
                                        cogl_matrix_scale (transform,
                                                           info->scale_x,
                                                           info->scale_y,
                                                           1.0));
        }

      if (info->rz_angle)
        TRANSFORM_ABOUT_ANCHOR_COORD (self, transform,
                                      &info->rz_center,
                                      cogl_matrix_rotate (transform,
                                                          info->rz_angle,
                                                          0, 0, 1.0));

      if (info->ry_angle)
        TRANSFORM_ABOUT_ANCHOR_COORD (self, transform,
                                      &info->ry_center,
                                      cogl_matrix_rotate (transform,
                                                          info->ry_angle,
                                                          0, 1.0, 0));

      if (info->rx_angle)
        TRANSFORM_ABOUT_ANCHOR_COORD (self, transform,
                                      &info->rx_center,
                                      cogl_matrix_rotate (transform,
                                                          info->rx_angle,
                                                          1.0, 0, 0));

      if (!clutter_anchor_coord_is_zero (&info->anchor))
        {
          gfloat x, y, z;

          clutter_anchor_coord_get_units (self, &info->anchor, &x, &y, &z);
          cogl_matrix_translate (transform, -x, -y, -z);
        }

      priv->transform_valid = TRUE;
    }

  cogl_matrix_multiply (matrix, matrix, &priv->transform);
}

/* Applies the transforms associated with this actor to the given
 * matrix. */
void
_clutter_actor_apply_modelview_transform (ClutterActor *self,
                                          CoglMatrix *matrix)
{
  CLUTTER_ACTOR_GET_CLASS (self)->apply_transform (self, matrix);
}

/*
 * clutter_actor_apply_relative_transformation_matrix:
 * @self: The actor whose coordinate space you want to transform from.
 * @ancestor: The ancestor actor whose coordinate space you want to transform too
 *            or %NULL if you want to transform all the way to eye coordinates.
 * @matrix: A #CoglMatrix to apply the transformation too.
 *
 * This multiplies a transform with @matrix that will transform coordinates
 * from the coordinate space of @self into the coordinate space of @ancestor.
 *
 * For example if you need a matrix that can transform the local actor
 * coordinates of @self into stage coordinates you would pass the actor's stage
 * pointer as the @ancestor.
 *
 * If you pass %NULL then the transformation will take you all the way through
 * to eye coordinates. This can be useful if you want to extract the entire
 * modelview transform that Clutter applies before applying the projection
 * transformation. If you want to explicitly set a modelview on a CoglFramebuffer
 * using cogl_set_modelview_matrix() for example then you would want a matrix
 * that transforms into eye coordinates.
 *
 * <note>This function doesn't initialize the given @matrix, it simply
 * multiplies the requested transformation matrix with the existing contents of
 * @matrix. You can use cogl_matrix_init_identity() to initialize the @matrix
 * before calling this function, or you can use
 * clutter_actor_get_relative_transformation_matrix() instead.</note>
 */
void
_clutter_actor_apply_relative_transformation_matrix (ClutterActor *self,
                                                     ClutterActor *ancestor,
                                                     CoglMatrix *matrix)
{
  ClutterActor *parent;

  /* Note we terminate before ever calling stage->apply_transform()
   * since that would conceptually be relative to the underlying
   * window OpenGL coordinates so we'd need a special @ancestor
   * value to represent the fake parent of the stage. */
  if (self == ancestor)
    return;

  parent = clutter_actor_get_parent (self);

  if (parent != NULL)
    _clutter_actor_apply_relative_transformation_matrix (parent, ancestor,
                                                         matrix);

  _clutter_actor_apply_modelview_transform (self, matrix);
}

static void
_clutter_actor_draw_paint_volume_full (ClutterActor *self,
                                       ClutterPaintVolume *pv,
                                       const char *label,
                                       const CoglColor *color)
{
  static CoglPipeline *outline = NULL;
  CoglPrimitive *prim;
  ClutterVertex line_ends[12 * 2];
  int n_vertices;
  CoglContext *ctx =
    clutter_backend_get_cogl_context (clutter_get_default_backend ());
  /* XXX: at some point we'll query this from the stage but we can't
   * do that until the osx backend uses Cogl natively. */
  CoglFramebuffer *fb = cogl_get_draw_framebuffer ();

  if (outline == NULL)
    outline = cogl_pipeline_new (ctx);

  _clutter_paint_volume_complete (pv);

  n_vertices = pv->is_2d ? 4 * 2 : 12 * 2;

  /* Front face */
  line_ends[0] = pv->vertices[0]; line_ends[1] = pv->vertices[1];
  line_ends[2] = pv->vertices[1]; line_ends[3] = pv->vertices[2];
  line_ends[4] = pv->vertices[2]; line_ends[5] = pv->vertices[3];
  line_ends[6] = pv->vertices[3]; line_ends[7] = pv->vertices[0];

  if (!pv->is_2d)
    {
      /* Back face */
      line_ends[8] = pv->vertices[4]; line_ends[9] = pv->vertices[5];
      line_ends[10] = pv->vertices[5]; line_ends[11] = pv->vertices[6];
      line_ends[12] = pv->vertices[6]; line_ends[13] = pv->vertices[7];
      line_ends[14] = pv->vertices[7]; line_ends[15] = pv->vertices[4];

      /* Lines connecting front face to back face */
      line_ends[16] = pv->vertices[0]; line_ends[17] = pv->vertices[4];
      line_ends[18] = pv->vertices[1]; line_ends[19] = pv->vertices[5];
      line_ends[20] = pv->vertices[2]; line_ends[21] = pv->vertices[6];
      line_ends[22] = pv->vertices[3]; line_ends[23] = pv->vertices[7];
    }

  prim = cogl_primitive_new_p3 (ctx, COGL_VERTICES_MODE_LINES,
                                n_vertices,
                                (CoglVertexP3 *)line_ends);

  cogl_pipeline_set_color (outline, color);
  cogl_framebuffer_draw_primitive (fb, outline, prim);
  cogl_object_unref (prim);

  if (label)
    {
      PangoLayout *layout;
      layout = pango_layout_new (clutter_actor_get_pango_context (self));
      pango_layout_set_text (layout, label, -1);
      cogl_pango_render_layout (layout,
                                pv->vertices[0].x,
                                pv->vertices[0].y,
                                color,
                                0);
      g_object_unref (layout);
    }
}

static void
_clutter_actor_draw_paint_volume (ClutterActor *self)
{
  ClutterPaintVolume *pv;
  CoglColor color;

  pv = _clutter_actor_get_paint_volume_mutable (self);
  if (!pv)
    {
      gfloat width, height;
      ClutterPaintVolume fake_pv;

      ClutterActor *stage = _clutter_actor_get_stage_internal (self);
      _clutter_paint_volume_init_static (&fake_pv, stage);

      clutter_actor_get_size (self, &width, &height);
      clutter_paint_volume_set_width (&fake_pv, width);
      clutter_paint_volume_set_height (&fake_pv, height);

      cogl_color_init_from_4f (&color, 0, 0, 1, 1);
      _clutter_actor_draw_paint_volume_full (self, &fake_pv,
                                             _clutter_actor_get_debug_name (self),
                                             &color);

      clutter_paint_volume_free (&fake_pv);
    }
  else
    {
      cogl_color_init_from_4f (&color, 0, 1, 0, 1);
      _clutter_actor_draw_paint_volume_full (self, pv,
                                             _clutter_actor_get_debug_name (self),
                                             &color);
    }
}

static void
_clutter_actor_paint_cull_result (ClutterActor *self,
                                  gboolean success,
                                  ClutterCullResult result)
{
  ClutterPaintVolume *pv;
  CoglColor color;

  if (success)
    {
      if (result == CLUTTER_CULL_RESULT_IN)
        cogl_color_init_from_4f (&color, 0, 1, 0, 1);
      else if (result == CLUTTER_CULL_RESULT_OUT)
        cogl_color_init_from_4f (&color, 0, 0, 1, 1);
      else
        cogl_color_init_from_4f (&color, 0, 1, 1, 1);
    }
  else
    cogl_color_init_from_4f (&color, 1, 1, 1, 1);

  if (success && (pv = _clutter_actor_get_paint_volume_mutable (self)))
    _clutter_actor_draw_paint_volume_full (self, pv,
                                           _clutter_actor_get_debug_name (self),
                                           &color);
  else
    {
      PangoLayout *layout;
      char *label =
        g_strdup_printf ("CULL FAILURE: %s", _clutter_actor_get_debug_name (self));
      cogl_color_init_from_4f (&color, 1, 1, 1, 1);
      cogl_set_source_color (&color);

      layout = pango_layout_new (clutter_actor_get_pango_context (self));
      pango_layout_set_text (layout, label, -1);
      cogl_pango_render_layout (layout,
                                0,
                                0,
                                &color,
                                0);
      g_free (label);
      g_object_unref (layout);
    }
}

static int clone_paint_level = 0;

void
_clutter_actor_push_clone_paint (void)
{
  clone_paint_level++;
}

void
_clutter_actor_pop_clone_paint (void)
{
  clone_paint_level--;
}

static gboolean
in_clone_paint (void)
{
  return clone_paint_level > 0;
}

/* Returns TRUE if the actor can be ignored */
/* FIXME: we should return a ClutterCullResult, and
 * clutter_actor_paint should understand that a CLUTTER_CULL_RESULT_IN
 * means there's no point in trying to cull descendants of the current
 * node. */
static gboolean
cull_actor (ClutterActor *self, ClutterCullResult *result_out)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterActor *stage;
  const ClutterPlane *stage_clip;

  if (!priv->last_paint_volume_valid)
    {
      CLUTTER_NOTE (CLIPPING, "Bail from cull_actor without culling (%s): "
                    "->last_paint_volume_valid == FALSE",
                    _clutter_actor_get_debug_name (self));
      return FALSE;
    }

  if (G_UNLIKELY (clutter_paint_debug_flags & CLUTTER_DEBUG_DISABLE_CULLING))
    return FALSE;

  stage = _clutter_actor_get_stage_internal (self);
  stage_clip = _clutter_stage_get_clip (CLUTTER_STAGE (stage));
  if (G_UNLIKELY (!stage_clip))
    {
      CLUTTER_NOTE (CLIPPING, "Bail from cull_actor without culling (%s): "
                    "No stage clip set",
                    _clutter_actor_get_debug_name (self));
      return FALSE;
    }

  if (cogl_get_draw_framebuffer () !=
      _clutter_stage_get_active_framebuffer (CLUTTER_STAGE (stage)))
    {
      CLUTTER_NOTE (CLIPPING, "Bail from cull_actor without culling (%s): "
                    "Current framebuffer doesn't correspond to stage",
                    _clutter_actor_get_debug_name (self));
      return FALSE;
    }

  *result_out =
    _clutter_paint_volume_cull (&priv->last_paint_volume, stage_clip);
  return TRUE;
}

static void
_clutter_actor_update_last_paint_volume (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;
  const ClutterPaintVolume *pv;

  if (priv->last_paint_volume_valid)
    {
      clutter_paint_volume_free (&priv->last_paint_volume);
      priv->last_paint_volume_valid = FALSE;
    }

  pv = clutter_actor_get_paint_volume (self);
  if (!pv)
    {
      CLUTTER_NOTE (CLIPPING, "Bail from update_last_paint_volume (%s): "
                    "Actor failed to report a paint volume",
                    _clutter_actor_get_debug_name (self));
      return;
    }

  _clutter_paint_volume_copy_static (pv, &priv->last_paint_volume);

  _clutter_paint_volume_transform_relative (&priv->last_paint_volume,
                                            NULL); /* eye coordinates */

  priv->last_paint_volume_valid = TRUE;
}

static inline gboolean
actor_has_shader_data (ClutterActor *self)
{
  return g_object_get_qdata (G_OBJECT (self), quark_shader_data) != NULL;
}

guint32
_clutter_actor_get_pick_id (ClutterActor *self)
{
  if (self->priv->pick_id < 0)
    return 0;

  return self->priv->pick_id;
}

/* This is the same as clutter_actor_add_effect except that it doesn't
   queue a redraw and it doesn't notify on the effect property */
static void
_clutter_actor_add_effect_internal (ClutterActor  *self,
                                    ClutterEffect *effect)
{
  ClutterActorPrivate *priv = self->priv;

  if (priv->effects == NULL)
    {
      priv->effects = g_object_new (CLUTTER_TYPE_META_GROUP, NULL);
      priv->effects->actor = self;
    }

  _clutter_meta_group_add_meta (priv->effects, CLUTTER_ACTOR_META (effect));
}

/* This is the same as clutter_actor_remove_effect except that it doesn't
   queue a redraw and it doesn't notify on the effect property */
static void
_clutter_actor_remove_effect_internal (ClutterActor  *self,
                                       ClutterEffect *effect)
{
  ClutterActorPrivate *priv = self->priv;

  if (priv->effects == NULL)
    return;

  _clutter_meta_group_remove_meta (priv->effects, CLUTTER_ACTOR_META (effect));

  if (_clutter_meta_group_peek_metas (priv->effects) == NULL)
    g_clear_object (&priv->effects);
}

static gboolean
needs_flatten_effect (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;

  if (G_UNLIKELY (clutter_paint_debug_flags &
                  CLUTTER_DEBUG_DISABLE_OFFSCREEN_REDIRECT))
    return FALSE;

  if (priv->offscreen_redirect & CLUTTER_OFFSCREEN_REDIRECT_ALWAYS)
    return TRUE;
  else if (priv->offscreen_redirect & CLUTTER_OFFSCREEN_REDIRECT_AUTOMATIC_FOR_OPACITY)
    {
      if (clutter_actor_get_paint_opacity (self) < 255 &&
          clutter_actor_has_overlaps (self))
        return TRUE;
    }

  return FALSE;
}

static void
add_or_remove_flatten_effect (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;

  /* Add or remove the flatten effect depending on the
     offscreen-redirect property. */
  if (needs_flatten_effect (self))
    {
      if (priv->flatten_effect == NULL)
        {
          ClutterActorMeta *actor_meta;
          gint priority;

          priv->flatten_effect = _clutter_flatten_effect_new ();
          /* Keep a reference to the effect so that we can queue
             redraws from it */
          g_object_ref_sink (priv->flatten_effect);

          /* Set the priority of the effect to high so that it will
             always be applied to the actor first. It uses an internal
             priority so that it won't be visible to applications */
          actor_meta = CLUTTER_ACTOR_META (priv->flatten_effect);
          priority = CLUTTER_ACTOR_META_PRIORITY_INTERNAL_HIGH;
          _clutter_actor_meta_set_priority (actor_meta, priority);

          /* This will add the effect without queueing a redraw */
          _clutter_actor_add_effect_internal (self, priv->flatten_effect);
        }
    }
  else
    {
      if (priv->flatten_effect != NULL)
        {
          /* Destroy the effect so that it will lose its fbo cache of
             the actor */
          _clutter_actor_remove_effect_internal (self, priv->flatten_effect);
          g_clear_object (&priv->flatten_effect);
        }
    }
}

static void
clutter_actor_real_paint (ClutterActor *actor)
{
  ClutterActorPrivate *priv = actor->priv;
  ClutterActor *iter;

  for (iter = priv->first_child;
       iter != NULL;
       iter = iter->priv->next_sibling)
    {
      const ClutterTransformInfo *info;

      info = _clutter_actor_get_transform_info_or_defaults (actor);

      CLUTTER_NOTE (PAINT, "Painting %s, child of %s, at { %.2f, %.2f - %.2f x %.2f }",
                    _clutter_actor_get_debug_name (iter),
                    _clutter_actor_get_debug_name (actor),
                    info->fixed_x, info->fixed_y,
                    info->width, info->height);

      clutter_actor_paint (iter);
    }
}

/**
 * clutter_actor_paint:
 * @self: A #ClutterActor
 *
 * Renders the actor to display.
 *
 * This function should not be called directly by applications.
 * Call clutter_actor_queue_redraw() to queue paints, instead.
 *
 * This function is context-aware, and will either cause a
 * regular paint or a pick paint.
 *
 * This function will emit the #ClutterActor::paint signal or
 * the #ClutterActor::pick signal, depending on the context.
 *
 * This function does not paint the actor if the actor is set to 0,
 * unless it is performing a pick paint.
 */
void
clutter_actor_paint (ClutterActor *self)
{
  ClutterActorPrivate *priv;
  ClutterPickMode pick_mode;
  gboolean clip_set = FALSE;
  gboolean shader_applied = FALSE;

  CLUTTER_STATIC_COUNTER (actor_paint_counter,
                          "Actor real-paint counter",
                          "Increments each time any actor is painted",
                          0 /* no application private data */);
  CLUTTER_STATIC_COUNTER (actor_pick_counter,
                          "Actor pick-paint counter",
                          "Increments each time any actor is painted "
                          "for picking",
                          0 /* no application private data */);

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (CLUTTER_ACTOR_IN_DESTRUCTION (self))
    return;

  priv = self->priv;

  pick_mode = _clutter_context_get_pick_mode ();

  if (pick_mode == CLUTTER_PICK_NONE)
    priv->propagated_one_redraw = FALSE;

  /* It's an important optimization that we consider painting of
   * actors with 0 opacity to be a NOP... */
  if (pick_mode == CLUTTER_PICK_NONE &&
      /* ignore top-levels, since they might be transparent */
      !CLUTTER_ACTOR_IS_TOPLEVEL (self) &&
      /* Use the override opacity if its been set */
      ((priv->opacity_override >= 0) ?
       priv->opacity_override : priv->opacity) == 0)
    return;

  /* if we aren't paintable (not in a toplevel with all
   * parents paintable) then do nothing.
   */
  if (!CLUTTER_ACTOR_IS_MAPPED (self))
    return;

  /* mark that we are in the paint process */
  CLUTTER_SET_PRIVATE_FLAGS (self, CLUTTER_IN_PAINT);

  cogl_push_matrix();

  if (priv->enable_model_view_transform)
    {
      CoglMatrix matrix;

      /* XXX: It could be better to cache the modelview with the actor
       * instead of progressively building up the transformations on
       * the matrix stack every time we paint. */
      cogl_get_modelview_matrix (&matrix);
      _clutter_actor_apply_modelview_transform (self, &matrix);

#ifdef CLUTTER_ENABLE_DEBUG
      /* Catch when out-of-band transforms have been made by actors not as part
       * of an apply_transform vfunc... */
      if (G_UNLIKELY (clutter_debug_flags & CLUTTER_DEBUG_OOB_TRANSFORMS))
        {
          CoglMatrix expected_matrix;

          _clutter_actor_get_relative_transformation_matrix (self, NULL,
                                                             &expected_matrix);

          if (!cogl_matrix_equal (&matrix, &expected_matrix))
            {
              GString *buf = g_string_sized_new (1024);
              ClutterActor *parent;

              parent = self;
              while (parent != NULL)
                {
                  g_string_append (buf, _clutter_actor_get_debug_name (parent));

                  if (parent->priv->parent != NULL)
                    g_string_append (buf, "->");

                  parent = parent->priv->parent;
                }

              g_warning ("Unexpected transform found when painting actor "
                         "\"%s\". This will be caused by one of the actor's "
                         "ancestors (%s) using the Cogl API directly to transform "
                         "children instead of using ::apply_transform().",
                         _clutter_actor_get_debug_name (self),
                         buf->str);

              g_string_free (buf, TRUE);
            }
        }
#endif /* CLUTTER_ENABLE_DEBUG */

      cogl_set_modelview_matrix (&matrix);
    }

  if (priv->has_clip)
    {
      cogl_clip_push_rectangle (priv->clip.x,
                                priv->clip.y,
                                priv->clip.x + priv->clip.width,
                                priv->clip.y + priv->clip.height);
      clip_set = TRUE;
    }

  if (pick_mode == CLUTTER_PICK_NONE)
    {
      CLUTTER_COUNTER_INC (_clutter_uprof_context, actor_paint_counter);

      /* We check whether we need to add the flatten effect before
         each paint so that we can avoid having a mechanism for
         applications to notify when the value of the
         has_overlaps virtual changes. */
      add_or_remove_flatten_effect (self);
    }
  else
    CLUTTER_COUNTER_INC (_clutter_uprof_context, actor_pick_counter);

  /* We save the current paint volume so that the next time the
   * actor queues a redraw we can constrain the redraw to just
   * cover the union of the new bounding box and the old.
   *
   * We also fetch the current paint volume to perform culling so
   * we can avoid painting actors outside the current clip region.
   *
   * If we are painting inside a clone, we should neither update
   * the paint volume or use it to cull painting, since the paint
   * box represents the location of the source actor on the
   * screen.
   *
   * XXX: We are starting to do a lot of vertex transforms on
   * the CPU in a typical paint, so at some point we should
   * audit these and consider caching some things.
   *
   * NB: We don't perform culling while picking at this point because
   * clutter-stage.c doesn't setup the clipping planes appropriately.
   *
   * NB: We don't want to update the last-paint-volume during picking
   * because the last-paint-volume is used to determine the old screen
   * space location of an actor that has moved so we can know the
   * minimal region to redraw to clear an old view of the actor. If we
   * update this during picking then by the time we come around to
   * paint then the last-paint-volume would likely represent the new
   * actor position not the old.
   */
  if (!in_clone_paint () && pick_mode == CLUTTER_PICK_NONE)
    {
      gboolean success;
      /* annoyingly gcc warns if uninitialized even though
       * the initialization is redundant :-( */
      ClutterCullResult result = CLUTTER_CULL_RESULT_IN;

      if (G_LIKELY ((clutter_paint_debug_flags &
                     (CLUTTER_DEBUG_DISABLE_CULLING |
                      CLUTTER_DEBUG_DISABLE_CLIPPED_REDRAWS)) !=
                    (CLUTTER_DEBUG_DISABLE_CULLING |
                     CLUTTER_DEBUG_DISABLE_CLIPPED_REDRAWS)))
        _clutter_actor_update_last_paint_volume (self);

      success = cull_actor (self, &result);

      if (G_UNLIKELY (clutter_paint_debug_flags & CLUTTER_DEBUG_REDRAWS))
        _clutter_actor_paint_cull_result (self, success, result);
      else if (result == CLUTTER_CULL_RESULT_OUT && success)
        goto done;
    }

  if (priv->effects == NULL)
    {
      if (pick_mode == CLUTTER_PICK_NONE &&
          actor_has_shader_data (self))
        {
          /* _clutter_actor_shader_pre_paint (self, FALSE); */
          /* shader_applied = TRUE; */
          /* TODO_LIONEL: fix this crap */
        }

      priv->next_effect_to_paint = NULL;
    }
  else
    priv->next_effect_to_paint =
      _clutter_meta_group_peek_metas (priv->effects);

  clutter_actor_continue_paint (self);

  /* if (shader_applied) */
  /*   _clutter_actor_shader_post_paint (self); */
  /* TODO_LIONEL: fix this crap */

  if (G_UNLIKELY (clutter_paint_debug_flags & CLUTTER_DEBUG_PAINT_VOLUMES &&
                  pick_mode == CLUTTER_PICK_NONE))
    _clutter_actor_draw_paint_volume (self);

done:
  /* If we make it here then the actor has run through a complete
     paint run including all the effects so it's no longer dirty */
  if (pick_mode == CLUTTER_PICK_NONE)
    priv->is_dirty = FALSE;

  if (clip_set)
    cogl_clip_pop();

  cogl_pop_matrix();

  /* paint sequence complete */
  CLUTTER_UNSET_PRIVATE_FLAGS (self, CLUTTER_IN_PAINT);
}

/**
 * clutter_actor_continue_paint:
 * @self: A #ClutterActor
 *
 * Run the next stage of the paint sequence. This function should only
 * be called within the implementation of the ‘run’ virtual of a
 * #ClutterEffect. It will cause the run method of the next effect to
 * be applied, or it will paint the actual actor if the current effect
 * is the last effect in the chain.
 *
 * Since: 1.8
 */
void
clutter_actor_continue_paint (ClutterActor *self)
{
  ClutterActorPrivate *priv;
  ClutterActorClass *klass;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  /* This should only be called from with in the ‘run’ implementation
     of a ClutterEffect */
  g_return_if_fail (CLUTTER_ACTOR_IN_PAINT (self));

  klass = CLUTTER_ACTOR_GET_CLASS (self);

  priv = self->priv;

  /* Skip any effects that are disabled */
  while (priv->next_effect_to_paint &&
         !clutter_actor_meta_get_enabled (priv->next_effect_to_paint->data))
    priv->next_effect_to_paint = priv->next_effect_to_paint->next;

  /* If this has come from the last effect then we'll just paint the
     actual actor */
  if (priv->next_effect_to_paint == NULL)
    {
      if (_clutter_context_get_pick_mode () == CLUTTER_PICK_NONE)
        {
          klass->paint (self);
        }
      else
        {
          ClutterColor col = { 0, };

          _clutter_id_to_color (_clutter_actor_get_pick_id (self), &col);

          /* Actor will then paint silhouette of itself in supplied
           * color.  See clutter_stage_get_actor_at_pos() for where
           * picking is enabled.
           */
          klass->pick (self, &col);
        }
    }
  else
    {
      ClutterEffect *old_current_effect;
      ClutterEffectPaintFlags run_flags = 0;

      /* Cache the current effect so that we can put it back before
         returning */
      old_current_effect = priv->current_effect;

      priv->current_effect = priv->next_effect_to_paint->data;
      priv->next_effect_to_paint = priv->next_effect_to_paint->next;

      if (_clutter_context_get_pick_mode () == CLUTTER_PICK_NONE)
        {
          if (priv->is_dirty)
            {
              /* If there's an effect queued with this redraw then all
                 effects up to that one will be considered dirty. It
                 is expected the queued effect will paint the cached
                 image and not call clutter_actor_continue_paint again
                 (although it should work ok if it does) */
              if (priv->effect_to_redraw == NULL ||
                  priv->current_effect != priv->effect_to_redraw)
                run_flags |= CLUTTER_EFFECT_PAINT_ACTOR_DIRTY;
            }

          _clutter_effect_paint (priv->current_effect, run_flags);
        }
      else
        {
          /* We can't determine when an actor has been modified since
             its last pick so lets just assume it has always been
             modified */
          run_flags |= CLUTTER_EFFECT_PAINT_ACTOR_DIRTY;

          _clutter_effect_pick (priv->current_effect, run_flags);
        }

      priv->current_effect = old_current_effect;
    }
}

static ClutterActorTraverseVisitFlags
invalidate_queue_redraw_entry (ClutterActor *self,
                               int           depth,
                               gpointer      user_data)
{
  ClutterActorPrivate *priv = self->priv;

  if (priv->queue_redraw_entry != NULL)
    {
      _clutter_stage_queue_redraw_entry_invalidate (priv->queue_redraw_entry);
      priv->queue_redraw_entry = NULL;
    }

  return CLUTTER_ACTOR_TRAVERSE_VISIT_CONTINUE;
}

static inline void
remove_child (ClutterActor *self,
              ClutterActor *child)
{
  ClutterActor *prev_sibling, *next_sibling;

  prev_sibling = child->priv->prev_sibling;
  next_sibling = child->priv->next_sibling;

  if (prev_sibling != NULL)
    prev_sibling->priv->next_sibling = next_sibling;

  if (next_sibling != NULL)
    next_sibling->priv->prev_sibling = prev_sibling;

  if (self->priv->first_child == child)
    self->priv->first_child = next_sibling;

  if (self->priv->last_child == child)
    self->priv->last_child = prev_sibling;

  child->priv->parent = NULL;
  child->priv->prev_sibling = NULL;
  child->priv->next_sibling = NULL;
}

typedef enum {
  REMOVE_CHILD_DESTROY_META       = 1 << 0,
  REMOVE_CHILD_EMIT_PARENT_SET    = 1 << 1,
  REMOVE_CHILD_EMIT_ACTOR_REMOVED = 1 << 2,
  REMOVE_CHILD_CHECK_STATE        = 1 << 3,
  REMOVE_CHILD_FLUSH_QUEUE        = 1 << 4,
  REMOVE_CHILD_NOTIFY_FIRST_LAST  = 1 << 5,

  /* default flags for public API */
  REMOVE_CHILD_DEFAULT_FLAGS      = REMOVE_CHILD_DESTROY_META |
                                    REMOVE_CHILD_EMIT_PARENT_SET |
                                    REMOVE_CHILD_EMIT_ACTOR_REMOVED |
                                    REMOVE_CHILD_CHECK_STATE |
                                    REMOVE_CHILD_FLUSH_QUEUE |
                                    REMOVE_CHILD_NOTIFY_FIRST_LAST,

  /* flags for legacy/deprecated API */
  REMOVE_CHILD_LEGACY_FLAGS       = REMOVE_CHILD_CHECK_STATE |
                                    REMOVE_CHILD_FLUSH_QUEUE |
                                    REMOVE_CHILD_EMIT_PARENT_SET |
                                    REMOVE_CHILD_NOTIFY_FIRST_LAST
} ClutterActorRemoveChildFlags;

/*< private >
 * clutter_actor_remove_child_internal:
 * @self: a #ClutterActor
 * @child: the child of @self that has to be removed
 * @flags: control the removal operations
 *
 * Removes @child from the list of children of @self.
 */
static void
clutter_actor_remove_child_internal (ClutterActor                 *self,
                                     ClutterActor                 *child,
                                     ClutterActorRemoveChildFlags  flags)
{
  ClutterActor *old_first, *old_last;
  gboolean destroy_meta, emit_parent_set, emit_actor_removed, check_state;
  gboolean flush_queue;
  gboolean notify_first_last;
  gboolean was_mapped;

  destroy_meta = (flags & REMOVE_CHILD_DESTROY_META) != 0;
  emit_parent_set = (flags & REMOVE_CHILD_EMIT_PARENT_SET) != 0;
  emit_actor_removed = (flags & REMOVE_CHILD_EMIT_ACTOR_REMOVED) != 0;
  check_state = (flags & REMOVE_CHILD_CHECK_STATE) != 0;
  flush_queue = (flags & REMOVE_CHILD_FLUSH_QUEUE) != 0;
  notify_first_last = (flags & REMOVE_CHILD_NOTIFY_FIRST_LAST) != 0;

  g_object_freeze_notify (G_OBJECT (self));

  if (destroy_meta)
    clutter_container_destroy_child_meta (CLUTTER_CONTAINER (self), child);

  if (check_state)
    {
      was_mapped = CLUTTER_ACTOR_IS_MAPPED (child);

      /* we need to unrealize *before* we set parent_actor to NULL,
       * because in an unrealize method actors are dissociating from the
       * stage, which means they need to be able to
       * clutter_actor_get_stage().
       *
       * yhis should unmap and unrealize, unless we're reparenting.
       */
      clutter_actor_update_map_state (child, MAP_STATE_MAKE_UNREALIZED);
    }
  else
    was_mapped = FALSE;

  if (flush_queue)
    {
      /* We take this opportunity to invalidate any queue redraw entry
       * associated with the actor and descendants since we won't be able to
       * determine the appropriate stage after this.
       *
       * we do this after we updated the mapped state because actors might
       * end up queueing redraws inside their mapped/unmapped virtual
       * functions, and if we invalidate the redraw entry we could end up
       * with an inconsistent state and weird memory corruption. see
       * bugs:
       *
       *   http://bugzilla.clutter-project.org/show_bug.cgi?id=2621
       *   https://bugzilla.gnome.org/show_bug.cgi?id=652036
       */
      _clutter_actor_traverse (child,
                               0,
                               invalidate_queue_redraw_entry,
                               NULL,
                               NULL);
    }

  old_first = self->priv->first_child;
  old_last = self->priv->last_child;

  remove_child (self, child);

  self->priv->n_children -= 1;

  self->priv->age += 1;

  /* clutter_actor_reparent() will emit ::parent-set for us */
  if (emit_parent_set && !CLUTTER_ACTOR_IN_REPARENT (child))
    g_signal_emit (child, actor_signals[PARENT_SET], 0, self);

  /* we need to emit the signal before dropping the reference */
  if (emit_actor_removed)
    g_signal_emit_by_name (self, "actor-removed", child);

  if (notify_first_last)
    {
      if (old_first != self->priv->first_child)
        g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_FIRST_CHILD]);

      if (old_last != self->priv->last_child)
        g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_LAST_CHILD]);
    }

  g_object_thaw_notify (G_OBJECT (self));

  /* remove the reference we acquired in clutter_actor_add_child() */
  g_object_unref (child);
}

static const ClutterTransformInfo default_transform_info = {
  0.0, 0.0,             /* fixed-x fixed-y */

  0.0, 0.0,             /* width, height */

  0.0, { 0, },          /* rotation-x */
  0.0, { 0, },          /* rotation-y */
  0.0, { 0, },          /* rotation-z */

  1.0, 1.0, { 0, },     /* scale */

  { 0, },               /* anchor */

  0.0,                  /* depth */

  { 0, },               /* color */
};

/*< private >
 * _clutter_actor_get_transform_info_or_defaults:
 * @self: a #ClutterActor
 *
 * Retrieves the ClutterTransformInfo structure associated to an actor.
 *
 * If the actor does not have a ClutterTransformInfo structure associated
 * to it, then the default structure will be returned.
 *
 * This function should only be used for getters.
 *
 * Return value: a const pointer to the ClutterTransformInfo structure
 */
const ClutterTransformInfo *
_clutter_actor_get_transform_info_or_defaults (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  info = ACTOR_GET_READ_DATA (self);
  /* info = g_object_get_qdata (G_OBJECT (self), quark_actor_transform_info); */
  if (info != NULL)
    return info;

  return &default_transform_info;
}

/* static void */
/* clutter_transform_info_free (gpointer data) */
/* { */
/*   if (data != NULL) */
/*     g_slice_free (ClutterTransformInfo, data); */
/* } */

/*< private >
 * _clutter_actor_get_transform_info:
 * @self: a #ClutterActor
 *
 * Retrieves a pointer to the ClutterTransformInfo structure.
 *
 * If the actor does not have a ClutterTransformInfo associated to it, one
 * will be created and initialized to the default values.
 *
 * This function should be used for setters.
 *
 * For getters, you should use _clutter_actor_get_transform_info_or_defaults()
 * instead.
 *
 * Return value: (transfer none): a pointer to the ClutterTransformInfo
 *   structure
 */
ClutterTransformInfo *
_clutter_actor_get_transform_info (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterTransformInfo *info;

  info = ACTOR_GET_WRITE_DATA (self);
  /* info = g_object_get_qdata (G_OBJECT (self), quark_actor_transform_info); */
  if (info == NULL)
    {
      priv->info = g_slice_new (ClutterTransformInfo);

      *priv->info = default_transform_info;

      /* g_object_set_qdata_full (G_OBJECT (self), quark_actor_transform_info, */
      /*                          info, */
      /*                          clutter_transform_info_free); */
    }

  return ACTOR_GET_WRITE_DATA (self);
}

/*< private >
 * clutter_actor_set_rotation_angle_internal:
 * @self: a #ClutterActor
 * @axis: the axis of the angle to change
 * @angle: the angle of rotation
 *
 * Sets the rotation angle on the given axis without affecting the
 * rotation center point.
 */
static inline void
clutter_actor_set_rotation_angle_internal (ClutterActor      *self,
                                           ClutterRotateAxis  axis,
                                           gdouble            angle)
{
  ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info (self);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      info->rx_angle = angle;
      break;

    case CLUTTER_Y_AXIS:
      info->ry_angle = angle;
      break;

    case CLUTTER_Z_AXIS:
      info->rz_angle = angle;
      break;
    }

  self->priv->transform_valid = FALSE;

  clutter_actor_queue_redraw (self);
}

static inline void
clutter_actor_set_rotation_angle (ClutterActor      *self,
                                  ClutterRotateAxis  axis,
                                  gdouble            angle)
{
  const ClutterTransformInfo *info;
  const double *cur_angle_p = NULL;
  GParamSpec *pspec = NULL;

  info = _clutter_actor_get_transform_info_or_defaults (self);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      cur_angle_p = &info->rx_angle;
      pspec = obj_anim_props[PROP_ROTATION_ANGLE_X];
      break;

    case CLUTTER_Y_AXIS:
      cur_angle_p = &info->ry_angle;
      pspec = obj_anim_props[PROP_ROTATION_ANGLE_Y];
      break;

    case CLUTTER_Z_AXIS:
      cur_angle_p = &info->rz_angle;
      pspec = obj_anim_props[PROP_ROTATION_ANGLE_Z];
      break;
    }

  g_assert (pspec != NULL);
  g_assert (cur_angle_p != NULL);

  if (_clutter_actor_get_transition (self, pspec) == NULL)
    _clutter_actor_create_transition (self, pspec, *cur_angle_p, angle);
  else
    _clutter_actor_update_transition (self, pspec, angle);

  clutter_actor_queue_redraw (self);
}

/*< private >
 * clutter_actor_set_rotation_center_internal:
 * @self: a #ClutterActor
 * @axis: the axis of the center to change
 * @center: the coordinates of the rotation center
 *
 * Sets the rotation center on the given axis without affecting the
 * rotation angle.
 */
static inline void
clutter_actor_set_rotation_center_internal (ClutterActor        *self,
                                            ClutterRotateAxis    axis,
                                            const ClutterVertex *center)
{
  GObject *obj = G_OBJECT (self);
  ClutterTransformInfo *info;
  ClutterVertex v = { 0, 0, 0 };

  info = _clutter_actor_get_transform_info (self);

  if (center != NULL)
    v = *center;

  g_object_freeze_notify (obj);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      clutter_anchor_coord_set_units (&info->rx_center, v.x, v.y, v.z);
      g_object_notify_by_pspec (obj, obj_props[PROP_ROTATION_CENTER_X]);
      break;

    case CLUTTER_Y_AXIS:
      clutter_anchor_coord_set_units (&info->ry_center, v.x, v.y, v.z);
      g_object_notify_by_pspec (obj, obj_props[PROP_ROTATION_CENTER_Y]);
      break;

    case CLUTTER_Z_AXIS:
      /* if the previously set rotation center was fractional, then
       * setting explicit coordinates will have to notify the
       * :rotation-center-z-gravity property as well
       */
      if (info->rz_center.is_fractional)
        g_object_notify_by_pspec (obj, obj_props[PROP_ROTATION_CENTER_Z_GRAVITY]);

      clutter_anchor_coord_set_units (&info->rz_center, v.x, v.y, v.z);
      g_object_notify_by_pspec (obj, obj_props[PROP_ROTATION_CENTER_Z]);
      break;
    }

  self->priv->transform_valid = FALSE;

  g_object_thaw_notify (obj);

  clutter_actor_queue_redraw (self);
}

static void
clutter_actor_set_scale_factor_internal (ClutterActor *self,
                                         double factor,
                                         GParamSpec *pspec)
{
  GObject *obj = G_OBJECT (self);
  ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info (self);

  if (pspec == obj_anim_props[PROP_SCALE_X])
    info->scale_x = factor;
  else
    info->scale_y = factor;

  self->priv->transform_valid = FALSE;
  clutter_actor_queue_redraw (self);
  g_object_notify_by_pspec (obj, pspec);
}

static inline void
clutter_actor_set_scale_factor (ClutterActor      *self,
                                ClutterRotateAxis  axis,
                                gdouble            factor)
{
  const ClutterTransformInfo *info;
  const double *scale_p = NULL;
  GParamSpec *pspec = NULL;

  info = _clutter_actor_get_transform_info_or_defaults (self);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      pspec = obj_anim_props[PROP_SCALE_X];
      scale_p = &info->scale_x;
      break;

    case CLUTTER_Y_AXIS:
      pspec = obj_anim_props[PROP_SCALE_Y];
      scale_p = &info->scale_y;
      break;

    case CLUTTER_Z_AXIS:
      break;
    }

  g_assert (pspec != NULL);
  g_assert (scale_p != NULL);

  if (_clutter_actor_get_transition (self, pspec) == NULL)
    _clutter_actor_create_transition (self, pspec, *scale_p, factor);
  else
    _clutter_actor_update_transition (self, pspec, factor);

  clutter_actor_queue_redraw (self);
}

static inline void
clutter_actor_set_scale_center (ClutterActor      *self,
                                ClutterRotateAxis  axis,
                                gfloat             coord)
{
  GObject *obj = G_OBJECT (self);
  ClutterTransformInfo *info;
  gfloat center_x, center_y;

  info = _clutter_actor_get_transform_info (self);

  g_object_freeze_notify (obj);

  /* get the current scale center coordinates */
  clutter_anchor_coord_get_units (self, &info->scale_center,
                                  &center_x,
                                  &center_y,
                                  NULL);

  /* we need to notify this too, because setting explicit coordinates will
   * change the gravity as a side effect
   */
  if (info->scale_center.is_fractional)
    g_object_notify_by_pspec (obj, obj_props[PROP_SCALE_GRAVITY]);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      clutter_anchor_coord_set_units (&info->scale_center, coord, center_y, 0);
      g_object_notify_by_pspec (obj, obj_props[PROP_SCALE_CENTER_X]);
      break;

    case CLUTTER_Y_AXIS:
      clutter_anchor_coord_set_units (&info->scale_center, center_x, coord, 0);
      g_object_notify_by_pspec (obj, obj_props[PROP_SCALE_CENTER_Y]);
      break;

    default:
      g_assert_not_reached ();
    }

  self->priv->transform_valid = FALSE;

  clutter_actor_queue_redraw (self);

  g_object_thaw_notify (obj);
}

static inline void
clutter_actor_set_scale_gravity (ClutterActor   *self,
                                 ClutterGravity  gravity)
{
  ClutterTransformInfo *info;
  GObject *obj;

  info = _clutter_actor_get_transform_info (self);
  obj = G_OBJECT (self);

  if (gravity == CLUTTER_GRAVITY_NONE)
    clutter_anchor_coord_set_units (&info->scale_center, 0, 0, 0);
  else
    clutter_anchor_coord_set_gravity (&info->scale_center, gravity);

  self->priv->transform_valid = FALSE;

  g_object_notify_by_pspec (obj, obj_props[PROP_SCALE_CENTER_X]);
  g_object_notify_by_pspec (obj, obj_props[PROP_SCALE_CENTER_Y]);
  g_object_notify_by_pspec (obj, obj_props[PROP_SCALE_GRAVITY]);

  clutter_actor_queue_redraw (self);
}

static inline void
clutter_actor_set_anchor_coord (ClutterActor      *self,
                                ClutterRotateAxis  axis,
                                gfloat             coord)
{
  GObject *obj = G_OBJECT (self);
  ClutterTransformInfo *info;
  gfloat anchor_x, anchor_y;

  info = _clutter_actor_get_transform_info (self);

  g_object_freeze_notify (obj);

  clutter_anchor_coord_get_units (self, &info->anchor,
                                  &anchor_x,
                                  &anchor_y,
                                  NULL);

  if (info->anchor.is_fractional)
    g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_GRAVITY]);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      clutter_anchor_coord_set_units (&info->anchor,
                                      coord,
                                      anchor_y,
                                      0.0);
      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_X]);
      break;

    case CLUTTER_Y_AXIS:
      clutter_anchor_coord_set_units (&info->anchor,
                                      anchor_x,
                                      coord,
                                      0.0);
      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_Y]);
      break;

    default:
      g_assert_not_reached ();
    }

  self->priv->transform_valid = FALSE;

  clutter_actor_queue_redraw (self);

  g_object_thaw_notify (obj);
}

static void
clutter_actor_set_property (GObject      *object,
			    guint         prop_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
  ClutterActor *actor = CLUTTER_ACTOR (object);
  ClutterActorPrivate *priv = actor->priv;

  switch (prop_id)
    {
    case PROP_OFFSCREEN_REDIRECT:
      clutter_actor_set_offscreen_redirect (actor, g_value_get_enum (value));
      break;

    case PROP_NAME:
      clutter_actor_set_name (actor, g_value_get_string (value));
      break;

    case PROP_VISIBLE:
      if (g_value_get_boolean (value) == TRUE)
	clutter_actor_show (actor);
      else
	clutter_actor_hide (actor);
      break;

    case PROP_SCALE_GRAVITY:
      clutter_actor_set_scale_gravity (actor, g_value_get_enum (value));
      break;

    case PROP_CLIP:
      {
        const ClutterGeometry *geom = g_value_get_boxed (value);

	clutter_actor_set_clip (actor,
				geom->x, geom->y,
				geom->width, geom->height);
      }
      break;

    case PROP_REACTIVE:
      clutter_actor_set_reactive (actor, g_value_get_boolean (value));
      break;

    case PROP_ROTATION_CENTER_Z_GRAVITY:
      {
        const ClutterTransformInfo *info;

        info = _clutter_actor_get_transform_info_or_defaults (actor);
        clutter_actor_set_z_rotation_from_gravity (actor, info->rz_angle,
                                                   g_value_get_enum (value));
      }
      break;

    case PROP_ANCHOR_X:
      clutter_actor_set_anchor_coord (actor, CLUTTER_X_AXIS,
                                      g_value_get_float (value));
      break;

    case PROP_ANCHOR_Y:
      clutter_actor_set_anchor_coord (actor, CLUTTER_Y_AXIS,
                                      g_value_get_float (value));
      break;

    case PROP_ANCHOR_GRAVITY:
      clutter_actor_set_anchor_point_from_gravity (actor,
                                                   g_value_get_enum (value));
      break;

    case PROP_SHOW_ON_SET_PARENT:
      priv->show_on_set_parent = g_value_get_boolean (value);
      break;

    case PROP_TEXT_DIRECTION:
      clutter_actor_set_text_direction (actor, g_value_get_enum (value));
      break;

    case PROP_EFFECT:
      clutter_actor_add_effect (actor, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
clutter_actor_get_property (GObject    *object,
			    guint       prop_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
  ClutterActor *actor = CLUTTER_ACTOR (object);
  ClutterActorPrivate *priv = actor->priv;

  switch (prop_id)
    {
    case PROP_OFFSCREEN_REDIRECT:
      g_value_set_enum (value, priv->offscreen_redirect);
      break;

    case PROP_NAME:
      g_value_set_string (value, priv->name);
      break;

    case PROP_VISIBLE:
      g_value_set_boolean (value, CLUTTER_ACTOR_IS_VISIBLE (actor));
      break;

    case PROP_MAPPED:
      g_value_set_boolean (value, CLUTTER_ACTOR_IS_MAPPED (actor));
      break;

    case PROP_REALIZED:
      g_value_set_boolean (value, CLUTTER_ACTOR_IS_REALIZED (actor));
      break;

    case PROP_HAS_CLIP:
      g_value_set_boolean (value, priv->has_clip);
      break;

    case PROP_CLIP:
      {
        ClutterGeometry clip;

        clip.x      = CLUTTER_NEARBYINT (priv->clip.x);
        clip.y      = CLUTTER_NEARBYINT (priv->clip.y);
        clip.width  = CLUTTER_NEARBYINT (priv->clip.width);
        clip.height = CLUTTER_NEARBYINT (priv->clip.height);

        g_value_set_boxed (value, &clip);
      }
      break;

    case PROP_SCALE_GRAVITY:
      g_value_set_enum (value, clutter_actor_get_scale_gravity (actor));
      break;

    case PROP_REACTIVE:
      g_value_set_boolean (value, clutter_actor_get_reactive (actor));
      break;

    case PROP_ROTATION_CENTER_Z_GRAVITY:
      g_value_set_enum (value, clutter_actor_get_z_rotation_gravity (actor));
      break;

    case PROP_ANCHOR_X:
      {
        const ClutterTransformInfo *info;
        gfloat anchor_x;

        info = _clutter_actor_get_transform_info_or_defaults (actor);
        clutter_anchor_coord_get_units (actor, &info->anchor,
                                        &anchor_x,
                                        NULL,
                                        NULL);
        g_value_set_float (value, anchor_x);
      }
      break;

    case PROP_ANCHOR_Y:
      {
        const ClutterTransformInfo *info;
        gfloat anchor_y;

        info = _clutter_actor_get_transform_info_or_defaults (actor);
        clutter_anchor_coord_get_units (actor, &info->anchor,
                                        NULL,
                                        &anchor_y,
                                        NULL);
        g_value_set_float (value, anchor_y);
      }
      break;

    case PROP_ANCHOR_GRAVITY:
      g_value_set_enum (value, clutter_actor_get_anchor_point_gravity (actor));
      break;

    case PROP_SHOW_ON_SET_PARENT:
      g_value_set_boolean (value, priv->show_on_set_parent);
      break;

    case PROP_TEXT_DIRECTION:
      g_value_set_enum (value, priv->text_direction);
      break;

    case PROP_HAS_POINTER:
      g_value_set_boolean (value, priv->has_pointer);
      break;

    case PROP_BACKGROUND_COLOR_SET:
      g_value_set_boolean (value, priv->bg_color_set);
      break;

    case PROP_FIRST_CHILD:
      g_value_set_object (value, priv->first_child);
      break;

    case PROP_LAST_CHILD:
      g_value_set_object (value, priv->last_child);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
clutter_actor_dispose (GObject *object)
{
  ClutterActor *self = CLUTTER_ACTOR (object);
  ClutterActorPrivate *priv = self->priv;
  ClutterActorClass *klass = CLUTTER_ACTOR_GET_CLASS (self);

  CLUTTER_NOTE (MISC, "Disposing of object (id=%d) of type '%s' (ref_count:%d)",
		priv->id,
		g_type_name (G_OBJECT_TYPE (self)),
                object->ref_count);

  g_signal_emit (self, actor_signals[DESTROY], 0);

  /* avoid recursing when called from clutter_actor_destroy() */
  if (priv->parent != NULL)
    {
      ClutterActor *parent = priv->parent;

      /* go through the Container implementation unless this
       * is an internal child and has been marked as such.
       *
       * removing the actor from its parent will reset the
       * realized and mapped states.
       */
      if (!CLUTTER_ACTOR_IS_INTERNAL_CHILD (self))
        clutter_actor_remove_child (parent, self);
      else
        clutter_actor_remove_child_internal (parent, self,
                                             REMOVE_CHILD_LEGACY_FLAGS);
    }

  /* parent must be gone at this point */
  g_assert (priv->parent == NULL);

  if (!CLUTTER_ACTOR_IS_TOPLEVEL (self))
    {
      /* can't be mapped or realized with no parent */
      g_assert (!CLUTTER_ACTOR_IS_MAPPED (self));
      g_assert (!CLUTTER_ACTOR_IS_REALIZED (self));
    }

  g_clear_object (&priv->pango_context);
  g_clear_object (&priv->effects);
  g_clear_object (&priv->flatten_effect);

  if (priv->info != NULL)
    {
      g_slice_free1 (klass->get_data_size (self), priv->info);
      priv->info = NULL;
    }

  G_OBJECT_CLASS (clutter_actor_parent_class)->dispose (object);
}

static void
clutter_actor_finalize (GObject *object)
{
  ClutterActorPrivate *priv = CLUTTER_ACTOR (object)->priv;

  CLUTTER_NOTE (MISC, "Finalize actor (name='%s', id=%d) of type '%s'",
                priv->name != NULL ? priv->name : "<none>",
		priv->id,
		g_type_name (G_OBJECT_TYPE (object)));

  _clutter_context_release_id (priv->id);

  g_free (priv->name);

  G_OBJECT_CLASS (clutter_actor_parent_class)->finalize (object);
}


/**
 * clutter_actor_get_accessible:
 * @self: a #ClutterActor
 *
 * Returns the accessible object that describes the actor to an
 * assistive technology.
 *
 * If no class-specific #AtkObject implementation is available for the
 * actor instance in question, it will inherit an #AtkObject
 * implementation from the first ancestor class for which such an
 * implementation is defined.
 *
 * The documentation of the <ulink
 * url="http://developer.gnome.org/doc/API/2.0/atk/index.html">ATK</ulink>
 * library contains more information about accessible objects and
 * their uses.
 *
 * Returns: (transfer none): the #AtkObject associated with @actor
 */
AtkObject *
clutter_actor_get_accessible (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return CLUTTER_ACTOR_GET_CLASS (self)->get_accessible (self);
}

static AtkObject *
clutter_actor_real_get_accessible (ClutterActor *actor)
{
  return atk_gobject_accessible_for_object (G_OBJECT (actor));
}

static AtkObject *
_clutter_actor_ref_accessible (AtkImplementor *implementor)
{
  AtkObject *accessible;

  accessible = clutter_actor_get_accessible (CLUTTER_ACTOR (implementor));
  if (accessible != NULL)
    g_object_ref (accessible);

  return accessible;
}

static void
atk_implementor_iface_init (AtkImplementorIface *iface)
{
  iface->ref_accessible = _clutter_actor_ref_accessible;
}

static gboolean
clutter_actor_update_default_paint_volume (ClutterActor       *self,
                                           ClutterPaintVolume *volume)
{
  ClutterActorPrivate *priv = self->priv;
  gboolean res = FALSE;
  ClutterActor *child;
  const ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info_or_defaults (self);

  /* we start from the allocation */
  clutter_paint_volume_set_width (volume, info->width);
  clutter_paint_volume_set_height (volume, info->height);

  /* if the actor has a clip set then we have a pretty definite
   * size for the paint volume: the actor cannot possibly paint
   * outside the clip region.
   */
  if (priv->has_clip &&
      priv->clip.width >= 0 &&
      priv->clip.height >= 0)
    {
      ClutterVertex origin;

      origin.x = priv->clip.x;
      origin.y = priv->clip.y;
      origin.z = 0;

      clutter_paint_volume_set_origin (volume, &origin);
      clutter_paint_volume_set_width (volume, priv->clip.width);
      clutter_paint_volume_set_height (volume, priv->clip.height);

      res = TRUE;
    }

  /* if we don't have children we just bail out here... */
  if (priv->n_children == 0)
    return res;

  /* ...but if we have children then we ask for their paint volume in
   * our coordinates. if any of our children replies that it doesn't
   * have a paint volume, we bail out
   */
  for (child  = priv->first_child;
       child != NULL;
       child  = child->priv->next_sibling)
    {
      const ClutterPaintVolume *child_volume;

      if (!CLUTTER_ACTOR_IS_MAPPED (child))
        continue;

      child_volume = clutter_actor_get_transformed_paint_volume (child, self);
      if (child_volume == NULL)
        {
          res = FALSE;
          break;
        }

      clutter_paint_volume_union (volume, child_volume);
      res = TRUE;
    }

  return res;

}

static gboolean
clutter_actor_real_get_paint_volume (ClutterActor       *self,
                                     ClutterPaintVolume *volume)
{
  ClutterActorClass *klass;
  gboolean res;

  klass = CLUTTER_ACTOR_GET_CLASS (self);

  /* XXX - this thoroughly sucks, but we don't want to penalize users
   * who use ClutterActor as a "new ClutterGroup" by forcing a full-stage
   * redraw. This should go away in 2.0.
   */
  if (klass->paint == clutter_actor_real_paint &&
      klass->get_paint_volume == clutter_actor_real_get_paint_volume)
    {
      res = TRUE;
    }
  else
    {
      /* this is the default return value: we cannot know if a class
       * is going to paint outside its allocation, so we take the
       * conservative approach.
       */
      res = FALSE;
    }

  if (clutter_actor_update_default_paint_volume (self, volume))
    return res;

  return FALSE;
}

/**
 * clutter_actor_get_default_paint_volume:
 * @self: a #ClutterActor
 *
 * Retrieves the default paint volume for @self.
 *
 * This function provides the same #ClutterPaintVolume that would be
 * computed by the default implementation inside #ClutterActor of the
 * #ClutterActorClass.get_paint_volume() virtual function.
 *
 * This function should only be used by #ClutterActor subclasses that
 * cannot chain up to the parent implementation when computing their
 * paint volume.
 *
 * Return value: (transfer none): a pointer to the default
 *   #ClutterPaintVolume, relative to the #ClutterActor, or %NULL if
 *   the actor could not compute a valid paint volume. The returned value
 *   is not guaranteed to be stable across multiple frames, so if you
 *   want to retain it, you will need to copy it using
 *   clutter_paint_volume_copy().
 *
 * Since: 1.10
 */
const ClutterPaintVolume *
clutter_actor_get_default_paint_volume (ClutterActor *self)
{
  ClutterPaintVolume volume;
  ClutterPaintVolume *res;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  res = NULL;
  _clutter_paint_volume_init_static (&volume, self);
  if (clutter_actor_update_default_paint_volume (self, &volume))
    {
      ClutterActor *stage = _clutter_actor_get_stage_internal (self);

      if (stage != NULL)
        {
          res = _clutter_stage_paint_volume_stack_allocate (CLUTTER_STAGE (stage));
          _clutter_paint_volume_copy_static (&volume, res);
        }
    }

  clutter_paint_volume_free (&volume);

  return res;
}

static gboolean
clutter_actor_real_has_overlaps (ClutterActor *self)
{
  /* By default we'll assume that all actors need an offscreen redirect to get
   * the correct opacity. Actors such as ClutterTexture that would never need
   * an offscreen redirect can override this to return FALSE. */
  return TRUE;
}

static void
clutter_actor_real_destroy (ClutterActor *actor)
{
  ClutterActorIter iter;

  g_object_freeze_notify (G_OBJECT (actor));

  clutter_actor_iter_init (&iter, actor);
  while (clutter_actor_iter_next (&iter, NULL))
    clutter_actor_iter_destroy (&iter);

  g_object_thaw_notify (G_OBJECT (actor));
}

static guint
clutter_actor_real_get_data_size (ClutterActor *actor)
{
  return sizeof (ClutterTransformInfo);
}

static inline guint
_clutter_actor_get_data_size (ClutterActor *actor)
{
  ClutterActorClass *klass = CLUTTER_ACTOR_GET_CLASS (actor);

  return klass->get_data_size (actor);
}

gpointer
_clutter_actor_get_read_data (ClutterActor *self)
{
  return self->priv->info;
}

gpointer
_clutter_actor_get_write_data (ClutterActor *self)
{
  return self->priv->info;
}

static void
clutter_actor_real_copy_data (ClutterActor *actor,
                              gpointer      dst,
                              gpointer      src)
{
  ClutterTransformInfo *tdst = (ClutterTransformInfo *) dst;
  ClutterTransformInfo *tsrc = (ClutterTransformInfo *) src;

  *tdst = *tsrc;
}

static GObject *
clutter_actor_constructor (GType gtype,
                           guint n_props,
                           GObjectConstructParam *props)
{
  GObjectClass *gobject_class;
  GObject *retval;

  gobject_class = G_OBJECT_CLASS (clutter_actor_parent_class);
  retval = gobject_class->constructor (gtype, n_props, props);

  return retval;
}

static void
clutter_actor_class_init (ClutterActorClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  quark_shader_data = g_quark_from_static_string ("-clutter-actor-shader-data");
  quark_actor_transform_info = g_quark_from_static_string ("-clutter-actor-transform-info");
  quark_actor_animation_info = g_quark_from_static_string ("-clutter-actor-animation-info");

  object_class->constructor = clutter_actor_constructor;
  object_class->set_property = clutter_actor_set_property;
  object_class->get_property = clutter_actor_get_property;
  object_class->dispose = clutter_actor_dispose;
  object_class->finalize = clutter_actor_finalize;

  klass->get_data_size = clutter_actor_real_get_data_size;
  klass->copy_data = clutter_actor_real_copy_data;

  klass->show = clutter_actor_real_show;
  klass->hide = clutter_actor_real_hide;
  klass->map = clutter_actor_real_map;
  klass->unmap = clutter_actor_real_unmap;
  klass->unrealize = clutter_actor_real_unrealize;
  klass->pick = clutter_actor_real_pick;
  klass->queue_redraw = clutter_actor_real_queue_redraw;
  klass->apply_transform = clutter_actor_real_apply_transform;
  klass->get_accessible = clutter_actor_real_get_accessible;
  klass->get_paint_volume = clutter_actor_real_get_paint_volume;
  klass->has_overlaps = clutter_actor_real_has_overlaps;
  klass->paint = clutter_actor_real_paint;
  klass->destroy = clutter_actor_real_destroy;

  g_type_class_add_private (klass, sizeof (ClutterActorPrivate));

  /**
   * ClutterActor:x:
   *
   * X coordinate of the actor in pixels. If written, forces a fixed
   * position for the actor. If read, returns the fixed position if any,
   * otherwise the allocation if available, otherwise 0.
   *
   * The #ClutterActor:x property is animatable.
   */
  obj_anim_props[PROP_X] =
    g_param_spec_float ("x",
                        P_("X coordinate"),
                        P_("X coordinate of the actor"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0.0,
                        G_PARAM_READWRITE |
                        G_PARAM_STATIC_STRINGS |
                        CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:y:
   *
   * Y coordinate of the actor in pixels. If written, forces a fixed
   * position for the actor.  If read, returns the fixed position if
   * any, otherwise the allocation if available, otherwise 0.
   *
   * The #ClutterActor:y property is animatable.
   */
  obj_anim_props[PROP_Y] =
    g_param_spec_float ("y",
                        P_("Y coordinate"),
                        P_("Y coordinate of the actor"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0.0,
                        G_PARAM_READWRITE |
                        G_PARAM_STATIC_STRINGS |
                        CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:width:
   *
   * Width of the actor (in pixels). If written, forces the minimum and
   * natural size request of the actor to the given width. If read, returns
   * the allocated width if available, otherwise the width request.
   *
   * The #ClutterActor:width property is animatable.
   */
  obj_anim_props[PROP_WIDTH] =
    g_param_spec_float ("width",
                        P_("Width"),
                        P_("Width of the actor"),
                        0.0, G_MAXFLOAT,
                        0.0,
                        G_PARAM_READWRITE |
                        G_PARAM_STATIC_STRINGS |
                        CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:height:
   *
   * Height of the actor (in pixels).  If written, forces the minimum and
   * natural size request of the actor to the given height. If read, returns
   * the allocated height if available, otherwise the height request.
   *
   * The #ClutterActor:height property is animatable.
   */
  obj_anim_props[PROP_HEIGHT] =
    g_param_spec_float ("height",
                        P_("Height"),
                        P_("Height of the actor"),
                        0.0, G_MAXFLOAT,
                        0.0,
                        G_PARAM_READWRITE |
                        G_PARAM_STATIC_STRINGS |
                        CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:depth:
   *
   * The position of the actor on the Z axis.
   *
   * The #ClutterActor:depth property is relative to the parent's
   * modelview matrix.
   *
   * The #ClutterActor:depth property is animatable.
   *
   * Since: 0.6
   */
  obj_anim_props[PROP_DEPTH] =
    g_param_spec_float ("depth",
                        P_("Depth"),
                        P_("Position on the Z axis"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0.0,
                        G_PARAM_READWRITE |
                        G_PARAM_STATIC_STRINGS |
                        CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:opacity:
   *
   * Opacity of an actor, between 0 (fully transparent) and
   * 255 (fully opaque)
   *
   * The #ClutterActor:opacity property is animatable.
   */
  obj_anim_props[PROP_OPACITY] =
    g_param_spec_uint ("opacity",
                       P_("Opacity"),
                       P_("Opacity of an actor"),
                       0, 255,
                       255,
                       G_PARAM_READWRITE |
                       G_PARAM_STATIC_STRINGS |
                       CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:scale-x:
   *
   * The horizontal scale of the actor.
   *
   * The #ClutterActor:scale-x property is animatable.
   *
   * Since: 0.6
   */
  obj_anim_props[PROP_SCALE_X] =
    g_param_spec_double ("scale-x",
                         P_("Scale X"),
                         P_("Scale factor on the X axis"),
                         0.0, G_MAXDOUBLE,
                         1.0,
                         G_PARAM_READWRITE |
                         G_PARAM_STATIC_STRINGS |
                         CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:scale-y:
   *
   * The vertical scale of the actor.
   *
   * The #ClutterActor:scale-y property is animatable.
   *
   * Since: 0.6
   */
  obj_anim_props[PROP_SCALE_Y] =
    g_param_spec_double ("scale-y",
                         P_("Scale Y"),
                         P_("Scale factor on the Y axis"),
                         0.0, G_MAXDOUBLE,
                         1.0,
                         G_PARAM_READWRITE |
                         G_PARAM_STATIC_STRINGS |
                         CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:rotation-angle-x:
   *
   * The rotation angle on the X axis.
   *
   * The #ClutterActor:rotation-angle-x property is animatable.
   *
   * Since: 0.6
   */
  obj_anim_props[PROP_ROTATION_ANGLE_X] =
    g_param_spec_double ("rotation-angle-x",
                         P_("Rotation Angle X"),
                         P_("The rotation angle on the X axis"),
                         -G_MAXDOUBLE, G_MAXDOUBLE,
                         0.0,
                         G_PARAM_READWRITE |
                         G_PARAM_STATIC_STRINGS |
                         CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:rotation-angle-y:
   *
   * The rotation angle on the Y axis
   *
   * The #ClutterActor:rotation-angle-y property is animatable.
   *
   * Since: 0.6
   */
  obj_anim_props[PROP_ROTATION_ANGLE_Y] =
    g_param_spec_double ("rotation-angle-y",
                         P_("Rotation Angle Y"),
                         P_("The rotation angle on the Y axis"),
                         -G_MAXDOUBLE, G_MAXDOUBLE,
                         0.0,
                         G_PARAM_READWRITE |
                         G_PARAM_STATIC_STRINGS |
                         CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:rotation-angle-z:
   *
   * The rotation angle on the Z axis
   *
   * The #ClutterActor:rotation-angle-z property is animatable.
   *
   * Since: 0.6
   */
  obj_anim_props[PROP_ROTATION_ANGLE_Z] =
    g_param_spec_double ("rotation-angle-z",
                         P_("Rotation Angle Z"),
                         P_("The rotation angle on the Z axis"),
                         -G_MAXDOUBLE, G_MAXDOUBLE,
                         0.0,
                         G_PARAM_READWRITE |
                         G_PARAM_STATIC_STRINGS |
                         CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:background-color:
   *
   * Paints a solid fill of the actor's allocation using the specified
   * color.
   *
   * The #ClutterActor:background-color property is animatable.
   *
   * Since: 1.10
   */
  obj_anim_props[PROP_BACKGROUND_COLOR] =
    clutter_param_spec_color ("background-color",
                              P_("Background color"),
                              P_("The actor's background color"),
                              CLUTTER_COLOR_Transparent,
                              G_PARAM_READWRITE |
                              G_PARAM_STATIC_STRINGS |
                              CLUTTER_PARAM_ANIMATABLE);

  /**
   * ClutterActor:offscreen-redirect:
   *
   * Determines the conditions in which the actor will be redirected
   * to an offscreen framebuffer while being painted. For example this
   * can be used to cache an actor in a framebuffer or for improved
   * handling of transparent actors. See
   * clutter_actor_set_offscreen_redirect() for details.
   *
   * Since: 1.8
   */
  obj_props[PROP_OFFSCREEN_REDIRECT] =
    g_param_spec_flags ("offscreen-redirect",
                        P_("Offscreen redirect"),
                        P_("Flags controlling when to flatten the actor into a single image"),
                        CLUTTER_TYPE_OFFSCREEN_REDIRECT,
                        0,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:visible:
   *
   * Whether the actor is set to be visible or not
   *
   * See also #ClutterActor:mapped
   */
  obj_props[PROP_VISIBLE] =
    g_param_spec_boolean ("visible",
                          P_("Visible"),
                          P_("Whether the actor is visible or not"),
                          FALSE,
                          CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:mapped:
   *
   * Whether the actor is mapped (will be painted when the stage
   * to which it belongs is mapped)
   *
   * Since: 1.0
   */
  obj_props[PROP_MAPPED] =
    g_param_spec_boolean ("mapped",
                          P_("Mapped"),
                          P_("Whether the actor will be painted"),
                          FALSE,
                          CLUTTER_PARAM_READABLE);

  /**
   * ClutterActor:realized:
   *
   * Whether the actor has been realized
   *
   * Since: 1.0
   */
  obj_props[PROP_REALIZED] =
    g_param_spec_boolean ("realized",
                          P_("Realized"),
                          P_("Whether the actor has been realized"),
                          FALSE,
                          CLUTTER_PARAM_READABLE);

  /**
   * ClutterActor:reactive:
   *
   * Whether the actor is reactive to events or not
   *
   * Only reactive actors will emit event-related signals
   *
   * Since: 0.6
   */
  obj_props[PROP_REACTIVE] =
    g_param_spec_boolean ("reactive",
                          P_("Reactive"),
                          P_("Whether the actor is reactive to events"),
                          FALSE,
                          CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:has-clip:
   *
   * Whether the actor has the #ClutterActor:clip property set or not
   */
  obj_props[PROP_HAS_CLIP] =
    g_param_spec_boolean ("has-clip",
                          P_("Has Clip"),
                          P_("Whether the actor has a clip set"),
                          FALSE,
                          CLUTTER_PARAM_READABLE);

  /**
   * ClutterActor:clip:
   *
   * The clip region for the actor, in actor-relative coordinates
   *
   * Every part of the actor outside the clip region will not be
   * painted
   */
  obj_props[PROP_CLIP] =
    g_param_spec_boxed ("clip",
                        P_("Clip"),
                        P_("The clip region for the actor"),
                        CLUTTER_TYPE_GEOMETRY,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:name:
   *
   * The name of the actor
   *
   * Since: 0.2
   */
  obj_props[PROP_NAME] =
    g_param_spec_string ("name",
                         P_("Name"),
                         P_("Name of the actor"),
                         NULL,
                         CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:scale-gravity:
   *
   * The center point for scaling expressed as a #ClutterGravity
   *
   * Since: 1.0
   */
  obj_props[PROP_SCALE_GRAVITY] =
    g_param_spec_enum ("scale-gravity",
                       P_("Scale Gravity"),
                       P_("The center of scaling"),
                       CLUTTER_TYPE_GRAVITY,
                       CLUTTER_GRAVITY_NONE,
                       CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:rotation-center-x:
   *
   * The rotation center on the X axis.
   *
   * Since: 0.6
   */
  obj_props[PROP_ROTATION_CENTER_X] =
    g_param_spec_boxed ("rotation-center-x",
                        P_("Rotation Center X"),
                        P_("The rotation center on the X axis"),
                        CLUTTER_TYPE_VERTEX,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:rotation-center-y:
   *
   * The rotation center on the Y axis.
   *
   * Since: 0.6
   */
  obj_props[PROP_ROTATION_CENTER_Y] =
    g_param_spec_boxed ("rotation-center-y",
                        P_("Rotation Center Y"),
                        P_("The rotation center on the Y axis"),
                        CLUTTER_TYPE_VERTEX,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:rotation-center-z:
   *
   * The rotation center on the Z axis.
   *
   * Since: 0.6
   */
  obj_props[PROP_ROTATION_CENTER_Z] =
    g_param_spec_boxed ("rotation-center-z",
                        P_("Rotation Center Z"),
                        P_("The rotation center on the Z axis"),
                        CLUTTER_TYPE_VERTEX,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:rotation-center-z-gravity:
   *
   * The rotation center on the Z axis expressed as a #ClutterGravity.
   *
   * Since: 1.0
   */
  obj_props[PROP_ROTATION_CENTER_Z_GRAVITY] =
    g_param_spec_enum ("rotation-center-z-gravity",
                       P_("Rotation Center Z Gravity"),
                       P_("Center point for rotation around the Z axis"),
                       CLUTTER_TYPE_GRAVITY,
                       CLUTTER_GRAVITY_NONE,
                       CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:scale-center-x:
   *
   * The horizontal center point for scaling
   *
   * Since: 1.0
   */
  obj_props[PROP_SCALE_CENTER_X] =
    g_param_spec_float ("scale-center-x",
                        P_("Scale Center X"),
                        P_("Horizontal scale center"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0.0,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:scale-center-y:
   *
   * The vertical center point for scaling
   *
   * Since: 1.0
   */
  obj_props[PROP_SCALE_CENTER_Y] =
    g_param_spec_float ("scale-center-y",
                        P_("Scale Center Y"),
                        P_("Vertical scale center"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0.0,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:anchor-x:
   *
   * The X coordinate of an actor's anchor point, relative to
   * the actor coordinate space, in pixels
   *
   * Since: 0.8
   */
  obj_props[PROP_ANCHOR_X] =
    g_param_spec_float ("anchor-x",
                        P_("Anchor X"),
                        P_("X coordinate of the anchor point"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:anchor-y:
   *
   * The Y coordinate of an actor's anchor point, relative to
   * the actor coordinate space, in pixels
   *
   * Since: 0.8
   */
  obj_props[PROP_ANCHOR_Y] =
    g_param_spec_float ("anchor-y",
                        P_("Anchor Y"),
                        P_("Y coordinate of the anchor point"),
                        -G_MAXFLOAT, G_MAXFLOAT,
                        0,
                        CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:anchor-gravity:
   *
   * The anchor point expressed as a #ClutterGravity
   *
   * Since: 1.0
   */
  obj_props[PROP_ANCHOR_GRAVITY] =
    g_param_spec_enum ("anchor-gravity",
                       P_("Anchor Gravity"),
                       P_("The anchor point as a ClutterGravity"),
                       CLUTTER_TYPE_GRAVITY,
                       CLUTTER_GRAVITY_NONE,
                       CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:show-on-set-parent:
   *
   * If %TRUE, the actor is automatically shown when parented.
   *
   * Calling clutter_actor_hide() on an actor which has not been
   * parented will set this property to %FALSE as a side effect.
   *
   * Since: 0.8
   */
  obj_props[PROP_SHOW_ON_SET_PARENT] =
    g_param_spec_boolean ("show-on-set-parent",
                          P_("Show on set parent"),
                          P_("Whether the actor is shown when parented"),
                          TRUE,
                          CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:text-direction:
   *
   * The direction of the text inside a #ClutterActor.
   *
   * Since: 1.0
   */
  obj_props[PROP_TEXT_DIRECTION] =
    g_param_spec_enum ("text-direction",
                       P_("Text Direction"),
                       P_("Direction of the text"),
                       CLUTTER_TYPE_TEXT_DIRECTION,
                       CLUTTER_TEXT_DIRECTION_LTR,
                       CLUTTER_PARAM_READWRITE);

  /**
   * ClutterActor:has-pointer:
   *
   * Whether the actor contains the pointer of a #ClutterInputDevice
   * or not.
   *
   * Since: 1.2
   */
  obj_props[PROP_HAS_POINTER] =
    g_param_spec_boolean ("has-pointer",
                          P_("Has Pointer"),
                          P_("Whether the actor contains the pointer of an input device"),
                          FALSE,
                          CLUTTER_PARAM_READABLE);

  /**
   * ClutterActor:effect:
   *
   * Adds #ClutterEffect to the list of effects be applied on a #ClutterActor
   *
   * Since: 1.4
   */
  obj_props[PROP_EFFECT] =
    g_param_spec_object ("effect",
                         P_("Effect"),
                         P_("Add an effect to be applied on the actor"),
                         CLUTTER_TYPE_EFFECT,
                         CLUTTER_PARAM_WRITABLE);

  /**
   * ClutterActor:background-color-set:
   *
   * Whether the #ClutterActor:background-color property has been set.
   *
   * Since: 1.10
   */
  obj_props[PROP_BACKGROUND_COLOR_SET] =
    g_param_spec_boolean ("background-color-set",
                          P_("Background Color Set"),
                          P_("Whether the background color is set"),
                          FALSE,
                          CLUTTER_PARAM_READABLE);

  /**
   * ClutterActor:first-child:
   *
   * The actor's first child.
   *
   * Since: 1.10
   */
  obj_props[PROP_FIRST_CHILD] =
    g_param_spec_object ("first-child",
                         P_("First Child"),
                         P_("The actor's first child"),
                         CLUTTER_TYPE_ACTOR,
                         CLUTTER_PARAM_READABLE);

  /**
   * ClutterActor:last-child:
   *
   * The actor's last child.
   *
   * Since: 1.10
   */
  obj_props[PROP_LAST_CHILD] =
    g_param_spec_object ("last-child",
                         P_("Last Child"),
                         P_("The actor's last child"),
                         CLUTTER_TYPE_ACTOR,
                         CLUTTER_PARAM_READABLE);

  g_object_class_install_properties (object_class, PROP_LAST, obj_props);
  clutter_animatable_install_properties (CLUTTER_TYPE_ACTOR,
                                         PROP_ANIMATABLE_LAST,
                                         obj_anim_props);


  /**
   * ClutterActor::destroy:
   * @actor: the #ClutterActor which emitted the signal
   *
   * The ::destroy signal notifies that all references held on the
   * actor which emitted it should be released.
   *
   * The ::destroy signal should be used by all holders of a reference
   * on @actor.
   *
   * This signal might result in the finalization of the #ClutterActor
   * if all references are released.
   *
   * Composite actors and actors implementing the #ClutterContainer
   * interface should override the default implementation of the
   * class handler of this signal and call clutter_actor_destroy() on
   * their children. When overriding the default class handler, it is
   * required to chain up to the parent's implementation.
   *
   * Since: 0.2
   */
  actor_signals[DESTROY] =
    g_signal_new (I_("destroy"),
		  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_CLEANUP | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		  G_STRUCT_OFFSET (ClutterActorClass, destroy),
		  NULL, NULL,
		  _clutter_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
  /**
   * ClutterActor::show:
   * @actor: the object which received the signal
   *
   * The ::show signal is emitted when an actor is visible and
   * rendered on the stage.
   *
   * Since: 0.2
   */
  actor_signals[SHOW] =
    g_signal_new (I_("show"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (ClutterActorClass, show),
		  NULL, NULL,
		  _clutter_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
  /**
   * ClutterActor::hide:
   * @actor: the object which received the signal
   *
   * The ::hide signal is emitted when an actor is no longer rendered
   * on the stage.
   *
   * Since: 0.2
   */
  actor_signals[HIDE] =
    g_signal_new (I_("hide"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (ClutterActorClass, hide),
		  NULL, NULL,
		  _clutter_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
  /**
   * ClutterActor::parent-set:
   * @actor: the object which received the signal
   * @old_parent: (allow-none): the previous parent of the actor, or %NULL
   *
   * This signal is emitted when the parent of the actor changes.
   *
   * Since: 0.2
   */
  actor_signals[PARENT_SET] =
    g_signal_new (I_("parent-set"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (ClutterActorClass, parent_set),
                  NULL, NULL,
                  _clutter_marshal_VOID__OBJECT,
                  G_TYPE_NONE, 1,
                  CLUTTER_TYPE_ACTOR);

  /**
   * ClutterActor::queue-redraw:
   * @actor: the actor we're bubbling the redraw request through
   * @origin: the actor which initiated the redraw request
   *
   * The ::queue_redraw signal is emitted when clutter_actor_queue_redraw()
   * is called on @origin.
   *
   * The default implementation for #ClutterActor chains up to the
   * parent actor and queues a redraw on the parent, thus "bubbling"
   * the redraw queue up through the actor graph. The default
   * implementation for #ClutterStage queues a clutter_stage_ensure_redraw()
   * in a main loop idle handler.
   *
   * Note that the @origin actor may be the stage, or a container; it
   * does not have to be a leaf node in the actor graph.
   *
   * Toolkits embedding a #ClutterStage which require a redraw and
   * relayout cycle can stop the emission of this signal using the
   * GSignal API, redraw the UI and then call clutter_stage_ensure_redraw()
   * themselves, like:
   *
   * |[
   *   static void
   *   on_redraw_complete (gpointer data)
   *   {
   *     ClutterStage *stage = data;
   *
   *     /&ast; execute the Clutter drawing pipeline &ast;/
   *     clutter_stage_ensure_redraw (stage);
   *   }
   *
   *   static void
   *   on_stage_queue_redraw (ClutterStage *stage)
   *   {
   *     /&ast; this prevents the default handler to run &ast;/
   *     g_signal_stop_emission_by_name (stage, "queue-redraw");
   *
   *     /&ast; queue a redraw with the host toolkit and call
   *      &ast; a function when the redraw has been completed
   *      &ast;/
   *     queue_a_redraw (G_CALLBACK (on_redraw_complete), stage);
   *   }
   * ]|
   *
   * <note><para>This signal is emitted before the Clutter paint
   * pipeline is executed. If you want to know when the pipeline has
   * been completed you should connect to the ::paint signal on the
   * Stage with g_signal_connect_after().</para></note>
   *
   * Since: 1.0
   */
  actor_signals[QUEUE_REDRAW] =
    g_signal_new (I_("queue-redraw"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST |
                  G_SIGNAL_NO_HOOKS,
		  G_STRUCT_OFFSET (ClutterActorClass, queue_redraw),
		  NULL, NULL,
		  _clutter_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
                  CLUTTER_TYPE_ACTOR);

  /**
   * ClutterActor::event:
   * @actor: the actor which received the event
   * @event: a #ClutterEvent
   *
   * The ::event signal is emitted each time an event is received
   * by the @actor. This signal will be emitted on every actor,
   * following the hierarchy chain, until it reaches the top-level
   * container (the #ClutterStage).
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[EVENT] =
    g_signal_new (I_("event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);
  /**
   * ClutterActor::button-press-event:
   * @actor: the actor which received the event
   * @event: (type ClutterButtonEvent): a #ClutterButtonEvent
   *
   * The ::button-press-event signal is emitted each time a mouse button
   * is pressed on @actor.
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[BUTTON_PRESS_EVENT] =
    g_signal_new (I_("button-press-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, button_press_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);
  /**
   * ClutterActor::button-release-event:
   * @actor: the actor which received the event
   * @event: (type ClutterButtonEvent): a #ClutterButtonEvent
   *
   * The ::button-release-event signal is emitted each time a mouse button
   * is released on @actor.
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[BUTTON_RELEASE_EVENT] =
    g_signal_new (I_("button-release-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, button_release_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);
  /**
   * ClutterActor::scroll-event:
   * @actor: the actor which received the event
   * @event: (type ClutterScrollEvent): a #ClutterScrollEvent
   *
   * The ::scroll-event signal is emitted each time the mouse is
   * scrolled on @actor
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[SCROLL_EVENT] =
    g_signal_new (I_("scroll-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, scroll_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);
  /**
   * ClutterActor::key-press-event:
   * @actor: the actor which received the event
   * @event: (type ClutterKeyEvent): a #ClutterKeyEvent
   *
   * The ::key-press-event signal is emitted each time a keyboard button
   * is pressed while @actor has key focus (see clutter_stage_set_key_focus()).
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[KEY_PRESS_EVENT] =
    g_signal_new (I_("key-press-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, key_press_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);
  /**
   * ClutterActor::key-release-event:
   * @actor: the actor which received the event
   * @event: (type ClutterKeyEvent): a #ClutterKeyEvent
   *
   * The ::key-release-event signal is emitted each time a keyboard button
   * is released while @actor has key focus (see
   * clutter_stage_set_key_focus()).
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[KEY_RELEASE_EVENT] =
    g_signal_new (I_("key-release-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, key_release_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);
  /**
   * ClutterActor::motion-event:
   * @actor: the actor which received the event
   * @event: (type ClutterMotionEvent): a #ClutterMotionEvent
   *
   * The ::motion-event signal is emitted each time the mouse pointer is
   * moved over @actor.
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[MOTION_EVENT] =
    g_signal_new (I_("motion-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, motion_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);

  /**
   * ClutterActor::key-focus-in:
   * @actor: the actor which now has key focus
   *
   * The ::key-focus-in signal is emitted when @actor receives key focus.
   *
   * Since: 0.6
   */
  actor_signals[KEY_FOCUS_IN] =
    g_signal_new (I_("key-focus-in"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, key_focus_in),
		  NULL, NULL,
		  _clutter_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  /**
   * ClutterActor::key-focus-out:
   * @actor: the actor which now has key focus
   *
   * The ::key-focus-out signal is emitted when @actor loses key focus.
   *
   * Since: 0.6
   */
  actor_signals[KEY_FOCUS_OUT] =
    g_signal_new (I_("key-focus-out"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, key_focus_out),
		  NULL, NULL,
		  _clutter_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  /**
   * ClutterActor::enter-event:
   * @actor: the actor which the pointer has entered.
   * @event: (type ClutterCrossingEvent): a #ClutterCrossingEvent
   *
   * The ::enter-event signal is emitted when the pointer enters the @actor
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[ENTER_EVENT] =
    g_signal_new (I_("enter-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, enter_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);

  /**
   * ClutterActor::leave-event:
   * @actor: the actor which the pointer has left
   * @event: (type ClutterCrossingEvent): a #ClutterCrossingEvent
   *
   * The ::leave-event signal is emitted when the pointer leaves the @actor.
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[LEAVE_EVENT] =
    g_signal_new (I_("leave-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, leave_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);

  /**
   * ClutterActor::captured-event:
   * @actor: the actor which received the signal
   * @event: a #ClutterEvent
   *
   * The ::captured-event signal is emitted when an event is captured
   * by Clutter. This signal will be emitted starting from the top-level
   * container (the #ClutterStage) to the actor which received the event
   * going down the hierarchy. This signal can be used to intercept every
   * event before the specialized events (like
   * ClutterActor::button-press-event or ::key-released-event) are
   * emitted.
   *
   * Return value: %TRUE if the event has been handled by the actor,
   *   or %FALSE to continue the emission.
   *
   * Since: 0.6
   */
  actor_signals[CAPTURED_EVENT] =
    g_signal_new (I_("captured-event"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ClutterActorClass, captured_event),
		  _clutter_boolean_handled_accumulator, NULL,
		  _clutter_marshal_BOOLEAN__BOXED,
		  G_TYPE_BOOLEAN, 1,
		  CLUTTER_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);

  /**
   * ClutterActor::realize:
   * @actor: the #ClutterActor that received the signal
   *
   * The ::realize signal is emitted each time an actor is being
   * realized.
   *
   * Since: 0.8
   */
  actor_signals[REALIZE] =
    g_signal_new (I_("realize"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (ClutterActorClass, realize),
                  NULL, NULL,
                  _clutter_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);
  /**
   * ClutterActor::unrealize:
   * @actor: the #ClutterActor that received the signal
   *
   * The ::unrealize signal is emitted each time an actor is being
   * unrealized.
   *
   * Since: 0.8
   */
  actor_signals[UNREALIZE] =
    g_signal_new (I_("unrealize"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (ClutterActorClass, unrealize),
                  NULL, NULL,
                  _clutter_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  /**
   * ClutterActor::transitions-completed:
   * @actor: a #ClutterActor
   *
   * The ::transitions-completed signal is emitted once all transitions
   * involving @actor are complete.
   *
   * Since: 1.10
   */
  actor_signals[TRANSITIONS_COMPLETED] =
    g_signal_new (I_("transitions-completed"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  _clutter_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);
}

static void
clutter_actor_init (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  self->priv = priv = CLUTTER_ACTOR_GET_PRIVATE (self);

  priv->id = _clutter_context_acquire_id (self);
  priv->pick_id = -1;

  priv->opacity = 0xff;
  priv->show_on_set_parent = TRUE;

  priv->opacity_override = -1;
  priv->enable_model_view_transform = TRUE;

  /* Initialize an empty paint volume to start with */
  _clutter_paint_volume_init_static (&priv->last_paint_volume, NULL);
  priv->last_paint_volume_valid = TRUE;

  priv->transform_valid = FALSE;
}

/**
 * clutter_actor_new:
 *
 * Creates a new #ClutterActor.
 *
 * A newly created actor has a floating reference, which will be sunk
 * when it is added to another actor.
 *
 * Return value: (transfer full): the newly created #ClutterActor
 *
 * Since: 1.10
 */
ClutterActor *
clutter_actor_new (void)
{
  return g_object_new (CLUTTER_TYPE_ACTOR, NULL);
}

/**
 * clutter_actor_destroy:
 * @self: a #ClutterActor
 *
 * Destroys an actor.  When an actor is destroyed, it will break any
 * references it holds to other objects.  If the actor is inside a
 * container, the actor will be removed.
 *
 * When you destroy a container, its children will be destroyed as well.
 *
 * Note: you cannot destroy the #ClutterStage returned by
 * clutter_stage_get_default().
 */
void
clutter_actor_destroy (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_object_ref (self);

  /* avoid recursion while destroying */
  if (!CLUTTER_ACTOR_IN_DESTRUCTION (self))
    {
      CLUTTER_SET_PRIVATE_FLAGS (self, CLUTTER_IN_DESTRUCTION);

      g_object_run_dispose (G_OBJECT (self));

      CLUTTER_UNSET_PRIVATE_FLAGS (self, CLUTTER_IN_DESTRUCTION);
    }

  g_object_unref (self);
}

void
_clutter_actor_finish_queue_redraw (ClutterActor *self,
                                    ClutterPaintVolume *clip)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterPaintVolume *pv;
  gboolean clipped;

  /* Remove queue entry early in the process, otherwise a new
     queue_redraw() during signal handling could put back this
     object in the stage redraw list (but the entry is freed as
     soon as we return from this function, causing a segfault
     later)
  */
  priv->queue_redraw_entry = NULL;

  /* If we've been explicitly passed a clip volume then there's
   * nothing more to calculate, but otherwise the only thing we know
   * is that the change is constrained to the given actor.
   *
   * The idea is that if we know the paint volume for where the actor
   * was last drawn (in eye coordinates) and we also have the paint
   * volume for where it will be drawn next (in actor coordinates)
   * then if we queue a redraw for both these volumes that will cover
   * everything that needs to be redrawn to clear the old view and
   * show the latest view of the actor.
   *
   * Don't clip this redraw if we don't know what position we had for
   * the previous redraw since we don't know where to set the clip so
   * it will clear the actor as it is currently.
   */
  if (clip)
    {
      _clutter_actor_set_queue_redraw_clip (self, clip);
      clipped = TRUE;
    }
  else if (G_LIKELY (priv->last_paint_volume_valid))
    {
      pv = _clutter_actor_get_paint_volume_mutable (self);
      if (pv)
        {
          ClutterActor *stage = _clutter_actor_get_stage_internal (self);

          /* make sure we redraw the actors old position... */
          _clutter_actor_set_queue_redraw_clip (stage,
                                                &priv->last_paint_volume);
          _clutter_actor_signal_queue_redraw (stage, stage);
          _clutter_actor_set_queue_redraw_clip (stage, NULL);

          /* XXX: Ideally the redraw signal would take a clip volume
           * argument, but that would be an ABI break. Until we can
           * break the ABI we pass the argument out-of-band
           */

          /* setup the clip for the actors new position... */
          _clutter_actor_set_queue_redraw_clip (self, pv);
          clipped = TRUE;
        }
      else
        clipped = FALSE;
    }
  else
    clipped = FALSE;

  _clutter_actor_signal_queue_redraw (self, self);

  /* Just in case anyone is manually firing redraw signals without
   * using the public queue_redraw() API we are careful to ensure that
   * our out-of-band clip member is cleared before returning...
   *
   * Note: A NULL clip denotes a full-stage, un-clipped redraw
   */
  if (G_LIKELY (clipped))
    _clutter_actor_set_queue_redraw_clip (self, NULL);
}

void
_clutter_actor_queue_redraw_full (ClutterActor       *self,
                                  ClutterRedrawFlags  flags,
                                  ClutterPaintVolume *volume,
                                  ClutterEffect      *effect)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterActor *stage;

  /* Here's an outline of the actor queue redraw mechanism:
   *
   * The process starts in one of the following two functions which
   * are wrappers for this function:
   * clutter_actor_queue_redraw
   * _clutter_actor_queue_redraw_with_clip
   *
   * additionally, an effect can queue a redraw by wrapping this
   * function in clutter_effect_queue_rerun
   *
   * This functions queues an entry in a list associated with the
   * stage which is a list of actors that queued a redraw while
   * updating the timelines, performing layouting and processing other
   * mainloop sources before the next paint starts.
   *
   * We aim to minimize the processing done at this point because
   * there is a good chance other events will happen while updating
   * the scenegraph that would invalidate any expensive work we might
   * otherwise try to do here. For example we don't try and resolve
   * the screen space bounding box of an actor at this stage so as to
   * minimize how much of the screen redraw because it's possible
   * something else will happen which will force a full redraw anyway.
   *
   * When all updates are complete and we come to paint the stage then
   * we iterate this list and actually emit the "queue-redraw" signals
   * for each of the listed actors which will bubble up to the stage
   * for each actor and at that point we will transform the actors
   * paint volume into screen coordinates to determine the clip region
   * for what needs to be redrawn in the next paint.
   *
   * Besides minimizing redundant work another reason for this
   * deferred design is that it's more likely we will be able to
   * determine the paint volume of an actor once we've finished
   * updating the scenegraph because its allocation should be up to
   * date. NB: If we can't determine an actors paint volume then we
   * can't automatically queue a clipped redraw which can make a big
   * difference to performance.
   *
   * So the control flow goes like this:
   * One of clutter_actor_queue_redraw,
   *        _clutter_actor_queue_redraw_with_clip
   *     or clutter_effect_queue_rerun
   *
   * then control moves to:
   *   _clutter_stage_queue_actor_redraw
   *
   * later during _clutter_stage_do_update, once relayouting is done
   * and the scenegraph has been updated we will call:
   * _clutter_stage_finish_queue_redraws
   *
   * _clutter_stage_finish_queue_redraws will call
   * _clutter_actor_finish_queue_redraw for each listed actor.
   * Note: actors *are* allowed to queue further redraws during this
   * process (considering clone actors or texture_new_from_actor which
   * respond to their source queueing a redraw by queuing a redraw
   * themselves). We repeat the process until the list is empty.
   *
   * This will result in the "queue-redraw" signal being fired for
   * each actor which will pass control to the default signal handler:
   * clutter_actor_real_queue_redraw
   *
   * This will bubble up to the stages handler:
   * clutter_stage_real_queue_redraw
   *
   * clutter_stage_real_queue_redraw will transform the actors paint
   * volume into screen space and add it as a clip region for the next
   * paint.
   */

  /* ignore queueing a redraw for actors being destroyed */
  if (CLUTTER_ACTOR_IN_DESTRUCTION (self))
    return;

  stage = _clutter_actor_get_stage_internal (self);

  /* Ignore queueing a redraw for actors not descended from a stage */
  if (stage == NULL)
    return;

  /* ignore queueing a redraw on stages that are being destroyed */
  if (CLUTTER_ACTOR_IN_DESTRUCTION (stage))
    return;

  self->priv->queue_redraw_entry =
    _clutter_stage_queue_actor_redraw (CLUTTER_STAGE (stage),
                                       priv->queue_redraw_entry,
                                       self,
                                       volume);

  /* If this is the first redraw queued then we can directly use the
     effect parameter */
  if (!priv->is_dirty)
    priv->effect_to_redraw = effect;
  /* Otherwise we need to merge it with the existing effect parameter */
  else if (effect != NULL)
    {
      /* If there's already an effect then we need to use whichever is
         later in the chain of actors. Otherwise a full redraw has
         already been queued on the actor so we need to ignore the
         effect parameter */
      if (priv->effect_to_redraw != NULL)
        {
          if (priv->effects == NULL)
            g_warning ("Redraw queued with an effect that is "
                       "not applied to the actor");
          else
            {
              const GList *l;

              for (l = _clutter_meta_group_peek_metas (priv->effects);
                   l != NULL;
                   l = l->next)
                {
                  if (l->data == priv->effect_to_redraw ||
                      l->data == effect)
                    priv->effect_to_redraw = l->data;
                }
            }
        }
    }
  else
    {
      /* If no effect is specified then we need to redraw the whole
         actor */
      priv->effect_to_redraw = NULL;
    }

  priv->is_dirty = TRUE;
}

/**
 * clutter_actor_queue_redraw:
 * @self: A #ClutterActor
 *
 * Queues up a redraw of an actor and any children. The redraw occurs
 * once the main loop becomes idle (after the current batch of events
 * has been processed, roughly).
 *
 * Applications rarely need to call this, as redraws are handled
 * automatically by modification functions.
 *
 * This function will not do anything if @self is not visible, or
 * if the actor is inside an invisible part of the scenegraph.
 *
 * Also be aware that painting is a NOP for actors with an opacity of
 * 0
 *
 * When you are implementing a custom actor you must queue a redraw
 * whenever some private state changes that will affect painting or
 * picking of your actor.
 */
void
clutter_actor_queue_redraw (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  _clutter_actor_queue_redraw_full (self,
                                    0, /* flags */
                                    NULL, /* clip volume */
                                    NULL /* effect */);
}

/*< private >
 * _clutter_actor_queue_redraw_with_clip:
 * @self: A #ClutterActor
 * @flags: A mask of #ClutterRedrawFlags controlling the behaviour of
 *   this queue redraw.
 * @volume: A #ClutterPaintVolume describing the bounds of what needs to be
 *   redrawn or %NULL if you are just using a @flag to state your
 *   desired clipping.
 *
 * Queues up a clipped redraw of an actor and any children. The redraw
 * occurs once the main loop becomes idle (after the current batch of
 * events has been processed, roughly).
 *
 * If no flags are given the clip volume is defined by @volume
 * specified in actor coordinates and tells Clutter that only content
 * within this volume has been changed so Clutter can optionally
 * optimize the redraw.
 *
 * If the %CLUTTER_REDRAW_CLIPPED_TO_ALLOCATION @flag is used, @volume
 * should be %NULL and this tells Clutter to use the actor's current
 * allocation as a clip box. This flag can only be used for 2D actors,
 * because any actor with depth may be projected outside its
 * allocation.
 *
 * Applications rarely need to call this, as redraws are handled
 * automatically by modification functions.
 *
 * This function will not do anything if @self is not visible, or if
 * the actor is inside an invisible part of the scenegraph.
 *
 * Also be aware that painting is a NOP for actors with an opacity of
 * 0
 *
 * When you are implementing a custom actor you must queue a redraw
 * whenever some private state changes that will affect painting or
 * picking of your actor.
 */
void
_clutter_actor_queue_redraw_with_clip (ClutterActor       *self,
                                       ClutterRedrawFlags  flags,
                                       ClutterPaintVolume *volume)
{
  _clutter_actor_queue_redraw_full (self,
                                    flags, /* flags */
                                    volume, /* clip volume */
                                    NULL /* effect */);
}

/**
 * clutter_actor_queue_redraw_with_clip:
 * @self: a #ClutterActor
 * @clip: (allow-none): a rectangular clip region, or %NULL
 *
 * Queues a redraw on @self limited to a specific, actor-relative
 * rectangular area.
 *
 * If @clip is %NULL this function is equivalent to
 * clutter_actor_queue_redraw().
 *
 * Since: 1.10
 */
void
clutter_actor_queue_redraw_with_clip (ClutterActor                *self,
                                      const cairo_rectangle_int_t *clip)
{
  ClutterPaintVolume volume;
  ClutterVertex origin;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (clip == NULL)
    {
      clutter_actor_queue_redraw (self);
      return;
    }

  _clutter_paint_volume_init_static (&volume, self);

  origin.x = clip->x;
  origin.y = clip->y;
  origin.z = 0.0f;

  clutter_paint_volume_set_origin (&volume, &origin);
  clutter_paint_volume_set_width (&volume, clip->width);
  clutter_paint_volume_set_height (&volume, clip->height);

  _clutter_actor_queue_redraw_full (self, 0, &volume, NULL);

  clutter_paint_volume_free (&volume);
}

/**
 * clutter_actor_set_position:
 * @self: A #ClutterActor
 * @x: New left position of actor in pixels.
 * @y: New top position of actor in pixels.
 *
 * Sets the actor's fixed position in pixels relative to any parent
 * actor.
 *
 * If a layout manager is in use, this position will override the
 * layout manager and force a fixed position.
 */
void
clutter_actor_set_position (ClutterActor *self,
			    gfloat        x,
			    gfloat        y)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_x (self, x);
  clutter_actor_set_y (self, y);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_move_by:
 * @self: A #ClutterActor
 * @dx: Distance to move Actor on X axis.
 * @dy: Distance to move Actor on Y axis.
 *
 * Moves an actor by the specified distance relative to its current
 * position in pixels.
 *
 * This function modifies the fixed position of an actor and thus removes
 * it from any layout management. Another way to move an actor is with an
 * anchor point, see clutter_actor_set_anchor_point().
 *
 * Since: 0.2
 */
void
clutter_actor_move_by (ClutterActor *self,
		       gfloat        dx,
		       gfloat        dy)
{
  const ClutterTransformInfo *info;
  gfloat x, y;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_transform_info_or_defaults (self);
  x = info->fixed_x;
  y = info->fixed_y;

  clutter_actor_set_position (self, x + dx, y + dy);
}

/* variant of set_width() without checks and without notification
 * freeze+thaw, for internal usage only
 */
static inline void
clutter_actor_set_width_internal (ClutterActor *self,
                                  gfloat        width)
{
  ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info (self);

  info->width = width;
}

/* variant of set_height() without checks and without notification
 * freeze+thaw, for internal usage only
 */
static inline void
clutter_actor_set_height_internal (ClutterActor *self,
                                   gfloat        height)
{
  ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info (self);

  info->height = height;
}

/**
 * clutter_actor_set_size:
 * @self: A #ClutterActor
 * @width: New width of actor in pixels, or -1
 * @height: New height of actor in pixels, or -1
 *
 * Sets the actor's size request in pixels. This overrides any
 * "normal" size request the actor would have. For example
 * a text actor might normally request the size of the text;
 * this function would force a specific size instead.
 *
 * If @width and/or @height are -1 the actor will use its
 * "normal" size request instead of overriding it, i.e.
 * you can "unset" the size with -1.
 *
 * This function sets or unsets both the minimum and natural size.
 */
void
clutter_actor_set_size (ClutterActor *self,
			gfloat        width,
			gfloat        height)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_width (self, width);
  clutter_actor_set_height (self, height);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_get_size:
 * @self: A #ClutterActor
 * @width: (out) (allow-none): return location for the width, or %NULL.
 * @height: (out) (allow-none): return location for the height, or %NULL.
 *
 * This function tries to "do what you mean" and return
 * the size an actor will have. If the actor has a valid
 * allocation, the allocation will be returned; otherwise,
 * the actors natural size request will be returned.
 *
 * If you care whether you get the request vs. the allocation, you
 * should probably call a different function like
 * clutter_actor_get_allocation_box() or
 * clutter_actor_get_preferred_width().
 *
 * Since: 0.2
 */
void
clutter_actor_get_size (ClutterActor *self,
			gfloat       *width,
			gfloat       *height)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (width)
    *width = clutter_actor_get_width (self);

  if (height)
    *height = clutter_actor_get_height (self);
}

/**
 * clutter_actor_get_position:
 * @self: a #ClutterActor
 * @x: (out) (allow-none): return location for the X coordinate, or %NULL
 * @y: (out) (allow-none): return location for the Y coordinate, or %NULL
 *
 * This function tries to "do what you mean" and tell you where the
 * actor is, prior to any transformations. Retrieves the fixed
 * position of an actor in pixels, if one has been set; otherwise, if
 * the allocation is valid, returns the actor's allocated position;
 * otherwise, returns 0,0.
 *
 * The returned position is in pixels.
 *
 * Since: 0.6
 */
void
clutter_actor_get_position (ClutterActor *self,
                            gfloat       *x,
                            gfloat       *y)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (x)
    *x = clutter_actor_get_x (self);

  if (y)
    *y = clutter_actor_get_y (self);
}

/**
 * clutter_actor_get_transformed_position:
 * @self: A #ClutterActor
 * @x: (out) (allow-none): return location for the X coordinate, or %NULL
 * @y: (out) (allow-none): return location for the Y coordinate, or %NULL
 *
 * Gets the absolute position of an actor, in pixels relative to the stage.
 *
 * Since: 0.8
 */
void
clutter_actor_get_transformed_position (ClutterActor *self,
                                        gfloat       *x,
                                        gfloat       *y)
{
  ClutterVertex v1;
  ClutterVertex v2;

  v1.x = v1.y = v1.z = 0;
  clutter_actor_apply_transform_to_point (self, &v1, &v2);

  if (x)
    *x = v2.x;

  if (y)
    *y = v2.y;
}

/**
 * clutter_actor_get_transformed_size:
 * @self: A #ClutterActor
 * @width: (out) (allow-none): return location for the width, or %NULL
 * @height: (out) (allow-none): return location for the height, or %NULL
 *
 * Gets the absolute size of an actor in pixels, taking into account the
 * scaling factors.
 *
 * If the actor has a valid allocation, the allocated size will be used.
 * If the actor has not a valid allocation then the preferred size will
 * be transformed and returned.
 *
 * If you want the transformed allocation, see
 * clutter_actor_get_abs_allocation_vertices() instead.
 *
 * <note>When the actor (or one of its ancestors) is rotated around the
 * X or Y axis, it no longer appears as on the stage as a rectangle, but
 * as a generic quadrangle; in that case this function returns the size
 * of the smallest rectangle that encapsulates the entire quad. Please
 * note that in this case no assumptions can be made about the relative
 * position of this envelope to the absolute position of the actor, as
 * returned by clutter_actor_get_transformed_position(); if you need this
 * information, you need to use clutter_actor_get_abs_allocation_vertices()
 * to get the coords of the actual quadrangle.</note>
 *
 * Since: 0.8
 */
void
clutter_actor_get_transformed_size (ClutterActor *self,
                                    gfloat       *width,
                                    gfloat       *height)
{
  ClutterVertex v[4];
  gfloat x_min, x_max, y_min, y_max;
  gint i;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  /* TODO_LIONEL: use width/height*/
  clutter_actor_get_abs_allocation_vertices (self, v);

  x_min = x_max = v[0].x;
  y_min = y_max = v[0].y;

  for (i = 1; i < G_N_ELEMENTS (v); ++i)
    {
      if (v[i].x < x_min)
	x_min = v[i].x;

      if (v[i].x > x_max)
	x_max = v[i].x;

      if (v[i].y < y_min)
	y_min = v[i].y;

      if (v[i].y > y_max)
	y_max = v[i].y;
    }

  if (width)
    *width  = x_max - x_min;

  if (height)
    *height = y_max - y_min;
}

/**
 * clutter_actor_get_width:
 * @self: A #ClutterActor
 *
 * Retrieves the width of a #ClutterActor.
 *
 * If the actor has a valid allocation, this function will return the
 * width of the allocated area given to the actor.
 *
 * If the actor does not have a valid allocation, this function will
 * return the actor's natural width, that is the preferred width of
 * the actor.
 *
 * If you care whether you get the preferred width or the width that
 * has been assigned to the actor, you should probably call a different
 * function like clutter_actor_get_allocation_box() to retrieve the
 * allocated size or clutter_actor_get_preferred_width() to retrieve the
 * preferred width.
 *
 * If an actor has a fixed width, for instance a width that has been
 * assigned using clutter_actor_set_width(), the width returned will
 * be the same value.
 *
 * Return value: the width of the actor, in pixels
 */
gfloat
clutter_actor_get_width (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return info->width;
}

/**
 * clutter_actor_get_height:
 * @self: A #ClutterActor
 *
 * Retrieves the height of a #ClutterActor.
 *
 * If the actor has a valid allocation, this function will return the
 * height of the allocated area given to the actor.
 *
 * If the actor does not have a valid allocation, this function will
 * return the actor's natural height, that is the preferred height of
 * the actor.
 *
 * If you care whether you get the preferred height or the height that
 * has been assigned to the actor, you should probably call a different
 * function like clutter_actor_get_allocation_box() to retrieve the
 * allocated size or clutter_actor_get_preferred_height() to retrieve the
 * preferred height.
 *
 * If an actor has a fixed height, for instance a height that has been
 * assigned using clutter_actor_set_height(), the height returned will
 * be the same value.
 *
 * Return value: the height of the actor, in pixels
 */
gfloat
clutter_actor_get_height (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return info->height;
}

/**
 * clutter_actor_get_allocation_geometry:
 * @self: A #ClutterActor
 * @geom: (out): allocation geometry in pixels
 *
 * Gets the layout box an actor has been assigned.  The allocation can
 * only be assumed valid inside a paint() method; anywhere else, it
 * may be out-of-date.
 *
 * An allocation does not incorporate the actor's scale or anchor point;
 * those transformations do not affect layout, only rendering.
 *
 * The returned rectangle is in pixels.
 *
 * Since: 0.8
 */
void
clutter_actor_get_allocation_geometry (ClutterActor    *self,
                                       ClutterGeometry *geom)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (geom != NULL);

  geom->x = CLUTTER_NEARBYINT (clutter_actor_get_x (self));
  geom->y = CLUTTER_NEARBYINT (clutter_actor_get_y (self));
  geom->width = CLUTTER_NEARBYINT (clutter_actor_get_width (self));
  geom->height = CLUTTER_NEARBYINT (clutter_actor_get_height (self));
}

/**
 * clutter_actor_set_width:
 * @self: A #ClutterActor
 * @width: Requested new width for the actor, in pixels, or -1
 *
 * Forces a width on an actor, causing the actor's preferred width
 * and height (if any) to be ignored.
 *
 * If @width is -1 the actor will use its preferred width request
 * instead of overriding it, i.e. you can "unset" the width with -1.
 *
 * This function sets both the minimum and natural size of the actor.
 *
 * since: 0.2
 */
void
clutter_actor_set_width (ClutterActor *self,
                         gfloat        width)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (_clutter_actor_get_transition (self, obj_anim_props[PROP_WIDTH]) == NULL)
    {
      float cur_size;

      /* minor optimization: if we don't have a duration
       * then we can skip the get_width() below, to avoid
       * the chance of going through get_preferred_width()
       * just to jump to a new desired width.
       */
      if (clutter_actor_get_easing_duration (self) == 0)
        {
          clutter_actor_set_width_internal (self, width);

          return;
        }
      else
        cur_size = clutter_actor_get_width (self);

      _clutter_actor_create_transition (self,
                                        obj_anim_props[PROP_WIDTH],
                                        cur_size,
                                        width);
    }
  else
    _clutter_actor_update_transition (self, obj_anim_props[PROP_WIDTH], width);
}

/**
 * clutter_actor_set_height:
 * @self: A #ClutterActor
 * @height: Requested new height for the actor, in pixels, or -1
 *
 * Forces a height on an actor, causing the actor's preferred width
 * and height (if any) to be ignored.
 *
 * If @height is -1 the actor will use its preferred height instead of
 * overriding it, i.e. you can "unset" the height with -1.
 *
 * This function sets both the minimum and natural size of the actor.
 *
 * since: 0.2
 */
void
clutter_actor_set_height (ClutterActor *self,
                          gfloat        height)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (_clutter_actor_get_transition (self, obj_anim_props[PROP_HEIGHT]) == NULL)
    {
      float cur_size;

      /* see the comment in clutter_actor_set_width() above */
      if (clutter_actor_get_easing_duration (self) == 0)
        {
          clutter_actor_set_height_internal (self, height);

          return;
        }
      else
        cur_size = clutter_actor_get_height (self);

      _clutter_actor_create_transition (self,
                                        obj_anim_props[PROP_HEIGHT],
                                        cur_size,
                                        height);
    }
  else
    _clutter_actor_update_transition (self, obj_anim_props[PROP_HEIGHT], height);
}

static inline void
clutter_actor_set_x_internal (ClutterActor *self,
                              float         x)
{
  ClutterTransformInfo *info;
  ClutterActorBox old = { 0, };

  info = _clutter_actor_get_transform_info (self);

  if (info->fixed_x == x)
    return;

  clutter_actor_store_old_geometry (self, &old);

  info->fixed_x = x;

  clutter_actor_queue_redraw (self);
}

static inline void
clutter_actor_set_y_internal (ClutterActor *self,
                              float         y)
{
  ClutterTransformInfo *info;
  ClutterActorBox old = { 0, };

  info = _clutter_actor_get_transform_info (self);

  if (info->fixed_y == y)
    return;

  clutter_actor_store_old_geometry (self, &old);

  info->fixed_y = y;

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_set_x:
 * @self: a #ClutterActor
 * @x: the actor's position on the X axis
 *
 * Sets the actor's X coordinate, relative to its parent, in pixels.
 *
 * Overrides any layout manager and forces a fixed position for
 * the actor.
 *
 * The #ClutterActor:x property is animatable.
 *
 * Since: 0.6
 */
void
clutter_actor_set_x (ClutterActor *self,
                     gfloat        x)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (_clutter_actor_get_transition (self, obj_anim_props[PROP_X]) == NULL)
    {
      float cur_position = clutter_actor_get_x (self);

      _clutter_actor_create_transition (self, obj_anim_props[PROP_X],
                                        cur_position,
                                        x);
    }
  else
    _clutter_actor_update_transition (self, obj_anim_props[PROP_X], x);
}

/**
 * clutter_actor_set_y:
 * @self: a #ClutterActor
 * @y: the actor's position on the Y axis
 *
 * Sets the actor's Y coordinate, relative to its parent, in pixels.#
 *
 * Overrides any layout manager and forces a fixed position for
 * the actor.
 *
 * The #ClutterActor:y property is animatable.
 *
 * Since: 0.6
 */
void
clutter_actor_set_y (ClutterActor *self,
                     gfloat        y)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (_clutter_actor_get_transition (self, obj_anim_props[PROP_Y]) == NULL)
    {
      float cur_position = clutter_actor_get_y (self);

      _clutter_actor_create_transition (self, obj_anim_props[PROP_Y],
                                        cur_position,
                                        y);
    }
  else
    _clutter_actor_update_transition (self, obj_anim_props[PROP_Y], y);
}

/**
 * clutter_actor_get_x:
 * @self: A #ClutterActor
 *
 * Retrieves the X coordinate of a #ClutterActor.
 *
 * This function tries to "do what you mean", by returning the
 * correct value depending on the actor's state.
 *
 * If the actor has a valid allocation, this function will return
 * the X coordinate of the origin of the allocation box.
 *
 * If the actor has any fixed coordinate set using clutter_actor_set_x(),
 * clutter_actor_set_position() or clutter_actor_set_geometry(), this
 * function will return that coordinate.
 *
 * If both the allocation and a fixed position are missing, this function
 * will return 0.
 *
 * Return value: the X coordinate, in pixels, ignoring any
 *   transformation (i.e. scaling, rotation)
 */
gfloat
clutter_actor_get_x (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return info->fixed_x;
}

/**
 * clutter_actor_get_y:
 * @self: A #ClutterActor
 *
 * Retrieves the Y coordinate of a #ClutterActor.
 *
 * This function tries to "do what you mean", by returning the
 * correct value depending on the actor's state.
 *
 * If the actor has a valid allocation, this function will return
 * the Y coordinate of the origin of the allocation box.
 *
 * If the actor has any fixed coordinate set using clutter_actor_set_y(),
 * clutter_actor_set_position() or clutter_actor_set_geometry(), this
 * function will return that coordinate.
 *
 * If both the allocation and a fixed position are missing, this function
 * will return 0.
 *
 * Return value: the Y coordinate, in pixels, ignoring any
 *   transformation (i.e. scaling, rotation)
 */
gfloat
clutter_actor_get_y (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return info->fixed_y;
}

/**
 * clutter_actor_set_scale:
 * @self: A #ClutterActor
 * @scale_x: double factor to scale actor by horizontally.
 * @scale_y: double factor to scale actor by vertically.
 *
 * Scales an actor with the given factors. The scaling is relative to
 * the scale center and the anchor point. The scale center is
 * unchanged by this function and defaults to 0,0.
 *
 * The #ClutterActor:scale-x and #ClutterActor:scale-y properties are
 * animatable.
 *
 * Since: 0.2
 */
void
clutter_actor_set_scale (ClutterActor *self,
                         gdouble       scale_x,
                         gdouble       scale_y)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_scale_factor (self, CLUTTER_X_AXIS, scale_x);
  clutter_actor_set_scale_factor (self, CLUTTER_Y_AXIS, scale_y);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_set_scale_full:
 * @self: A #ClutterActor
 * @scale_x: double factor to scale actor by horizontally.
 * @scale_y: double factor to scale actor by vertically.
 * @center_x: X coordinate of the center of the scale.
 * @center_y: Y coordinate of the center of the scale
 *
 * Scales an actor with the given factors around the given center
 * point. The center point is specified in pixels relative to the
 * anchor point (usually the top left corner of the actor).
 *
 * The #ClutterActor:scale-x and #ClutterActor:scale-y properties
 * are animatable.
 *
 * Since: 1.0
 */
void
clutter_actor_set_scale_full (ClutterActor *self,
                              gdouble       scale_x,
                              gdouble       scale_y,
                              gfloat        center_x,
                              gfloat        center_y)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_scale_factor (self, CLUTTER_X_AXIS, scale_x);
  clutter_actor_set_scale_factor (self, CLUTTER_Y_AXIS, scale_y);
  clutter_actor_set_scale_center (self, CLUTTER_X_AXIS, center_x);
  clutter_actor_set_scale_center (self, CLUTTER_Y_AXIS, center_y);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_set_scale_with_gravity:
 * @self: A #ClutterActor
 * @scale_x: double factor to scale actor by horizontally.
 * @scale_y: double factor to scale actor by vertically.
 * @gravity: the location of the scale center expressed as a compass
 * direction.
 *
 * Scales an actor with the given factors around the given
 * center point. The center point is specified as one of the compass
 * directions in #ClutterGravity. For example, setting it to north
 * will cause the top of the actor to remain unchanged and the rest of
 * the actor to expand left, right and downwards.
 *
 * The #ClutterActor:scale-x and #ClutterActor:scale-y properties are
 * animatable.
 *
 * Since: 1.0
 */
void
clutter_actor_set_scale_with_gravity (ClutterActor   *self,
                                      gdouble         scale_x,
                                      gdouble         scale_y,
                                      ClutterGravity  gravity)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_scale_factor (self, CLUTTER_X_AXIS, scale_x);
  clutter_actor_set_scale_factor (self, CLUTTER_Y_AXIS, scale_y);
  clutter_actor_set_scale_gravity (self, gravity);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_get_scale:
 * @self: A #ClutterActor
 * @scale_x: (out) (allow-none): Location to store horizonal
 *   scale factor, or %NULL.
 * @scale_y: (out) (allow-none): Location to store vertical
 *   scale factor, or %NULL.
 *
 * Retrieves an actors scale factors.
 *
 * Since: 0.2
 */
void
clutter_actor_get_scale (ClutterActor *self,
			 gdouble      *scale_x,
			 gdouble      *scale_y)
{
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_transform_info_or_defaults (self);

  if (scale_x)
    *scale_x = info->scale_x;

  if (scale_y)
    *scale_y = info->scale_y;
}

/**
 * clutter_actor_get_scale_center:
 * @self: A #ClutterActor
 * @center_x: (out) (allow-none): Location to store the X position
 *   of the scale center, or %NULL.
 * @center_y: (out) (allow-none): Location to store the Y position
 *   of the scale center, or %NULL.
 *
 * Retrieves the scale center coordinate in pixels relative to the top
 * left corner of the actor. If the scale center was specified using a
 * #ClutterGravity this will calculate the pixel offset using the
 * current size of the actor.
 *
 * Since: 1.0
 */
void
clutter_actor_get_scale_center (ClutterActor *self,
                                gfloat       *center_x,
                                gfloat       *center_y)
{
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_transform_info_or_defaults (self);

  clutter_anchor_coord_get_units (self, &info->scale_center,
                                  center_x,
                                  center_y,
                                  NULL);
}

/**
 * clutter_actor_get_scale_gravity:
 * @self: A #ClutterActor
 *
 * Retrieves the scale center as a compass direction. If the scale
 * center was specified in pixels or units this will return
 * %CLUTTER_GRAVITY_NONE.
 *
 * Return value: the scale gravity
 *
 * Since: 1.0
 */
ClutterGravity
clutter_actor_get_scale_gravity (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), CLUTTER_GRAVITY_NONE);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return clutter_anchor_coord_get_gravity (&info->scale_center);
}

static inline void
clutter_actor_set_opacity_internal (ClutterActor *self,
                                    guint8        opacity)
{
  ClutterActorPrivate *priv = self->priv;

  if (priv->opacity != opacity)
    {
      priv->opacity = opacity;

      /* Queue a redraw from the flatten effect so that it can use
         its cached image if available instead of having to redraw the
         actual actor. If it doesn't end up using the FBO then the
         effect is still able to continue the paint anyway. If there
         is no flatten effect yet then this is equivalent to queueing
         a full redraw */
      _clutter_actor_queue_redraw_full (self,
                                        0, /* flags */
                                        NULL, /* clip */
                                        priv->flatten_effect);

      g_object_notify_by_pspec (G_OBJECT (self), obj_anim_props[PROP_OPACITY]);
    }
}

/**
 * clutter_actor_set_opacity:
 * @self: A #ClutterActor
 * @opacity: New opacity value for the actor.
 *
 * Sets the actor's opacity, with zero being completely transparent and
 * 255 (0xff) being fully opaque.
 *
 * The #ClutterActor:opacity property is animatable.
 */
void
clutter_actor_set_opacity (ClutterActor *self,
			   guint8        opacity)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (_clutter_actor_get_transition (self, obj_anim_props[PROP_OPACITY]) == NULL)
    {
      _clutter_actor_create_transition (self, obj_anim_props[PROP_OPACITY],
                                        self->priv->opacity,
                                        opacity);
    }
  else
    _clutter_actor_update_transition (self, obj_anim_props[PROP_OPACITY], opacity);
}

/*
 * clutter_actor_get_paint_opacity_internal:
 * @self: a #ClutterActor
 *
 * Retrieves the absolute opacity of the actor, as it appears on the stage
 *
 * This function does not do type checks
 *
 * Return value: the absolute opacity of the actor
 */
static guint8
clutter_actor_get_paint_opacity_internal (ClutterActor *self)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterActor *parent;

  /* override the top-level opacity to always be 255; even in
   * case of ClutterStage:use-alpha being TRUE we want the rest
   * of the scene to be painted
   */
  if (CLUTTER_ACTOR_IS_TOPLEVEL (self))
    return 255;

  if (priv->opacity_override >= 0)
    return priv->opacity_override;

  parent = priv->parent;

  /* Factor in the actual actors opacity with parents */
  if (parent != NULL)
    {
      guint8 opacity = clutter_actor_get_paint_opacity_internal (parent);

      if (opacity != 0xff)
        return (opacity * priv->opacity) / 0xff;
    }

  return priv->opacity;

}

/**
 * clutter_actor_get_paint_opacity:
 * @self: A #ClutterActor
 *
 * Retrieves the absolute opacity of the actor, as it appears on the stage.
 *
 * This function traverses the hierarchy chain and composites the opacity of
 * the actor with that of its parents.
 *
 * This function is intended for subclasses to use in the paint virtual
 * function, to paint themselves with the correct opacity.
 *
 * Return value: The actor opacity value.
 *
 * Since: 0.8
 */
guint8
clutter_actor_get_paint_opacity (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  return clutter_actor_get_paint_opacity_internal (self);
}

/**
 * clutter_actor_get_opacity:
 * @self: a #ClutterActor
 *
 * Retrieves the opacity value of an actor, as set by
 * clutter_actor_set_opacity().
 *
 * For retrieving the absolute opacity of the actor inside a paint
 * virtual function, see clutter_actor_get_paint_opacity().
 *
 * Return value: the opacity of the actor
 */
guint8
clutter_actor_get_opacity (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  return self->priv->opacity;
}

/**
 * clutter_actor_set_offscreen_redirect:
 * @self: A #ClutterActor
 * @redirect: New offscreen redirect flags for the actor.
 *
 * Defines the circumstances where the actor should be redirected into
 * an offscreen image. The offscreen image is used to flatten the
 * actor into a single image while painting for two main reasons.
 * Firstly, when the actor is painted a second time without any of its
 * contents changing it can simply repaint the cached image without
 * descending further down the actor hierarchy. Secondly, it will make
 * the opacity look correct even if there are overlapping primitives
 * in the actor.
 *
 * Caching the actor could in some cases be a performance win and in
 * some cases be a performance lose so it is important to determine
 * which value is right for an actor before modifying this value. For
 * example, there is never any reason to flatten an actor that is just
 * a single texture (such as a #ClutterTexture) because it is
 * effectively already cached in an image so the offscreen would be
 * redundant. Also if the actor contains primitives that are far apart
 * with a large transparent area in the middle (such as a large
 * CluterGroup with a small actor in the top left and a small actor in
 * the bottom right) then the cached image will contain the entire
 * image of the large area and the paint will waste time blending all
 * of the transparent pixels in the middle.
 *
 * The default method of implementing opacity on a container simply
 * forwards on the opacity to all of the children. If the children are
 * overlapping then it will appear as if they are two separate glassy
 * objects and there will be a break in the color where they
 * overlap. By redirecting to an offscreen buffer it will be as if the
 * two opaque objects are combined into one and then made transparent
 * which is usually what is expected.
 *
 * The image below demonstrates the difference between redirecting and
 * not. The image shows two Clutter groups, each containing a red and
 * a green rectangle which overlap. The opacity on the group is set to
 * 128 (which is 50%). When the offscreen redirect is not used, the
 * red rectangle can be seen through the blue rectangle as if the two
 * rectangles were separately transparent. When the redirect is used
 * the group as a whole is transparent instead so the red rectangle is
 * not visible where they overlap.
 *
 * <figure id="offscreen-redirect">
 *   <title>Sample of using an offscreen redirect for transparency</title>
 *   <graphic fileref="offscreen-redirect.png" format="PNG"/>
 * </figure>
 *
 * The default value for this property is 0, so we effectively will
 * never redirect an actor offscreen by default. This means that there
 * are times that transparent actors may look glassy as described
 * above. The reason this is the default is because there is a
 * performance trade off between quality and performance here. In many
 * cases the default form of glassy opacity looks good enough, but if
 * it's not you will need to set the
 * %CLUTTER_OFFSCREEN_REDIRECT_AUTOMATIC_FOR_OPACITY flag to enable
 * redirection for opacity.
 *
 * Custom actors that don't contain any overlapping primitives are
 * recommended to override the has_overlaps() virtual to return %FALSE
 * for maximum efficiency.
 *
 * Since: 1.8
 */
void
clutter_actor_set_offscreen_redirect (ClutterActor *self,
                                      ClutterOffscreenRedirect redirect)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  priv = self->priv;

  if (priv->offscreen_redirect != redirect)
    {
      priv->offscreen_redirect = redirect;

      /* Queue a redraw from the effect so that it can use its cached
         image if available instead of having to redraw the actual
         actor. If it doesn't end up using the FBO then the effect is
         still able to continue the paint anyway. If there is no
         effect then this is equivalent to queuing a full redraw */
      _clutter_actor_queue_redraw_full (self,
                                        0, /* flags */
                                        NULL, /* clip */
                                        priv->flatten_effect);

      g_object_notify_by_pspec (G_OBJECT (self),
                                obj_props[PROP_OFFSCREEN_REDIRECT]);
    }
}

/**
 * clutter_actor_get_offscreen_redirect:
 * @self: a #ClutterActor
 *
 * Retrieves whether to redirect the actor to an offscreen buffer, as
 * set by clutter_actor_set_offscreen_redirect().
 *
 * Return value: the value of the offscreen-redirect property of the actor
 *
 * Since: 1.8
 */
ClutterOffscreenRedirect
clutter_actor_get_offscreen_redirect (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  return self->priv->offscreen_redirect;
}

/**
 * clutter_actor_set_name:
 * @self: A #ClutterActor
 * @name: Textual tag to apply to actor
 *
 * Sets the given name to @self. The name can be used to identify
 * a #ClutterActor.
 */
void
clutter_actor_set_name (ClutterActor *self,
			const gchar  *name)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  g_free (self->priv->name);
  self->priv->name = g_strdup (name);

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_NAME]);
}

/**
 * clutter_actor_get_name:
 * @self: A #ClutterActor
 *
 * Retrieves the name of @self.
 *
 * Return value: the name of the actor, or %NULL. The returned string is
 *   owned by the actor and should not be modified or freed.
 */
const gchar *
clutter_actor_get_name (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return self->priv->name;
}

static inline void
clutter_actor_set_depth_internal (ClutterActor *self,
                                  float         depth)
{
  ClutterTransformInfo *info;

  info = _clutter_actor_get_transform_info (self);

  if (info->depth != depth)
    {
      /* Sets Z value - XXX 2.0: should we invert? */
      info->depth = depth;

      self->priv->transform_valid = FALSE;

      /* /\* FIXME - remove this crap; sadly, there are still containers */
      /*  * in Clutter that depend on this utter brain damage */
      /*  *\/ */
      /* clutter_container_sort_depth_order (CLUTTER_CONTAINER (self)); */
      /* TODO_LIONEL: fix this... */

      clutter_actor_queue_redraw (self);
    }
}

/**
 * clutter_actor_set_depth:
 * @self: a #ClutterActor
 * @depth: Z co-ord
 *
 * Sets the Z coordinate of @self to @depth.
 *
 * The unit used by @depth is dependant on the perspective setup. See
 * also clutter_stage_set_perspective().
 */
void
clutter_actor_set_depth (ClutterActor *self,
                         gfloat        depth)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (_clutter_actor_get_transition (self, obj_anim_props[PROP_DEPTH]) == NULL)
    {
      const ClutterTransformInfo *info;

      info = _clutter_actor_get_transform_info_or_defaults (self);

      _clutter_actor_create_transition (self, obj_anim_props[PROP_DEPTH],
                                        info->depth,
                                        depth);
    }
  else
    _clutter_actor_update_transition (self, obj_anim_props[PROP_DEPTH], depth);

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_get_depth:
 * @self: a #ClutterActor
 *
 * Retrieves the depth of @self.
 *
 * Return value: the depth of the actor
 */
gfloat
clutter_actor_get_depth (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0.0);

  return _clutter_actor_get_transform_info_or_defaults (self)->depth;
}

/**
 * clutter_actor_set_rotation:
 * @self: a #ClutterActor
 * @axis: the axis of rotation
 * @angle: the angle of rotation
 * @x: X coordinate of the rotation center
 * @y: Y coordinate of the rotation center
 * @z: Z coordinate of the rotation center
 *
 * Sets the rotation angle of @self around the given axis.
 *
 * The rotation center coordinates used depend on the value of @axis:
 * <itemizedlist>
 *   <listitem><para>%CLUTTER_X_AXIS requires @y and @z</para></listitem>
 *   <listitem><para>%CLUTTER_Y_AXIS requires @x and @z</para></listitem>
 *   <listitem><para>%CLUTTER_Z_AXIS requires @x and @y</para></listitem>
 * </itemizedlist>
 *
 * The rotation coordinates are relative to the anchor point of the
 * actor, set using clutter_actor_set_anchor_point(). If no anchor
 * point is set, the upper left corner is assumed as the origin.
 *
 * Since: 0.8
 */
void
clutter_actor_set_rotation (ClutterActor      *self,
                            ClutterRotateAxis  axis,
                            gdouble            angle,
                            gfloat             x,
                            gfloat             y,
                            gfloat             z)
{
  ClutterVertex v;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  v.x = x;
  v.y = y;
  v.z = z;

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_rotation_angle (self, axis, angle);
  clutter_actor_set_rotation_center_internal (self, axis, &v);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_set_z_rotation_from_gravity:
 * @self: a #ClutterActor
 * @angle: the angle of rotation
 * @gravity: the center point of the rotation
 *
 * Sets the rotation angle of @self around the Z axis using the center
 * point specified as a compass point. For example to rotate such that
 * the center of the actor remains static you can use
 * %CLUTTER_GRAVITY_CENTER. If the actor changes size the center point
 * will move accordingly.
 *
 * Since: 1.0
 */
void
clutter_actor_set_z_rotation_from_gravity (ClutterActor   *self,
                                           gdouble         angle,
                                           ClutterGravity  gravity)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (gravity == CLUTTER_GRAVITY_NONE)
    clutter_actor_set_rotation (self, CLUTTER_Z_AXIS, angle, 0, 0, 0);
  else
    {
      GObject *obj = G_OBJECT (self);
      ClutterTransformInfo *info;

      info = _clutter_actor_get_transform_info (self);

      g_object_freeze_notify (obj);

      clutter_actor_set_rotation_angle_internal (self, CLUTTER_Z_AXIS, angle);

      clutter_anchor_coord_set_gravity (&info->rz_center, gravity);
      g_object_notify_by_pspec (obj, obj_props[PROP_ROTATION_CENTER_Z_GRAVITY]);
      g_object_notify_by_pspec (obj, obj_props[PROP_ROTATION_CENTER_Z]);

      g_object_thaw_notify (obj);
    }
}

/**
 * clutter_actor_get_rotation:
 * @self: a #ClutterActor
 * @axis: the axis of rotation
 * @x: (out): return value for the X coordinate of the center of rotation
 * @y: (out): return value for the Y coordinate of the center of rotation
 * @z: (out): return value for the Z coordinate of the center of rotation
 *
 * Retrieves the angle and center of rotation on the given axis,
 * set using clutter_actor_set_rotation().
 *
 * Return value: the angle of rotation
 *
 * Since: 0.8
 */
gdouble
clutter_actor_get_rotation (ClutterActor      *self,
                            ClutterRotateAxis  axis,
                            gfloat            *x,
                            gfloat            *y,
                            gfloat            *z)
{
  const ClutterTransformInfo *info;
  const AnchorCoord *anchor_coord;
  gdouble retval = 0;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  switch (axis)
    {
    case CLUTTER_X_AXIS:
      anchor_coord = &info->rx_center;
      retval = info->rx_angle;
      break;

    case CLUTTER_Y_AXIS:
      anchor_coord = &info->ry_center;
      retval = info->ry_angle;
      break;

    case CLUTTER_Z_AXIS:
      anchor_coord = &info->rz_center;
      retval = info->rz_angle;
      break;

    default:
      anchor_coord = NULL;
      retval = 0.0;
      break;
    }

  clutter_anchor_coord_get_units (self, anchor_coord, x, y, z);

  return retval;
}

/**
 * clutter_actor_get_z_rotation_gravity:
 * @self: A #ClutterActor
 *
 * Retrieves the center for the rotation around the Z axis as a
 * compass direction. If the center was specified in pixels or units
 * this will return %CLUTTER_GRAVITY_NONE.
 *
 * Return value: the Z rotation center
 *
 * Since: 1.0
 */
ClutterGravity
clutter_actor_get_z_rotation_gravity (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0.0);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return clutter_anchor_coord_get_gravity (&info->rz_center);
}

/**
 * clutter_actor_set_clip:
 * @self: A #ClutterActor
 * @xoff: X offset of the clip rectangle
 * @yoff: Y offset of the clip rectangle
 * @width: Width of the clip rectangle
 * @height: Height of the clip rectangle
 *
 * Sets clip area for @self. The clip area is always computed from the
 * upper left corner of the actor, even if the anchor point is set
 * otherwise.
 *
 * Since: 0.6
 */
void
clutter_actor_set_clip (ClutterActor *self,
                        gfloat        xoff,
                        gfloat        yoff,
                        gfloat        width,
                        gfloat        height)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  priv = self->priv;

  if (priv->has_clip &&
      priv->clip.x == xoff &&
      priv->clip.y == yoff &&
      priv->clip.width == width &&
      priv->clip.height == height)
    return;

  priv->clip.x = xoff;
  priv->clip.y = yoff;
  priv->clip.width = width;
  priv->clip.height = height;

  priv->has_clip = TRUE;

  clutter_actor_queue_redraw (self);

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_HAS_CLIP]);
  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_CLIP]);
}

/**
 * clutter_actor_remove_clip:
 * @self: A #ClutterActor
 *
 * Removes clip area from @self.
 */
void
clutter_actor_remove_clip (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (!self->priv->has_clip)
    return;

  self->priv->has_clip = FALSE;

  clutter_actor_queue_redraw (self);

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_HAS_CLIP]);
}

/**
 * clutter_actor_has_clip:
 * @self: a #ClutterActor
 *
 * Determines whether the actor has a clip area set or not.
 *
 * Return value: %TRUE if the actor has a clip area set.
 *
 * Since: 0.1.1
 */
gboolean
clutter_actor_has_clip (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  return self->priv->has_clip;
}

/**
 * clutter_actor_get_clip:
 * @self: a #ClutterActor
 * @xoff: (out) (allow-none): return location for the X offset of
 *   the clip rectangle, or %NULL
 * @yoff: (out) (allow-none): return location for the Y offset of
 *   the clip rectangle, or %NULL
 * @width: (out) (allow-none): return location for the width of
 *   the clip rectangle, or %NULL
 * @height: (out) (allow-none): return location for the height of
 *   the clip rectangle, or %NULL
 *
 * Gets the clip area for @self, if any is set
 *
 * Since: 0.6
 */
void
clutter_actor_get_clip (ClutterActor *self,
                        gfloat       *xoff,
                        gfloat       *yoff,
                        gfloat       *width,
                        gfloat       *height)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  priv = self->priv;

  if (!priv->has_clip)
    return;

  if (xoff != NULL)
    *xoff = priv->clip.x;

  if (yoff != NULL)
    *yoff = priv->clip.y;

  if (width != NULL)
    *width = priv->clip.width;

  if (height != NULL)
    *height = priv->clip.height;
}

/**
 * clutter_actor_get_children:
 * @self: a #ClutterActor
 *
 * Retrieves the list of children of @self.
 *
 * Return value: (transfer container) (element-type ClutterActor): A newly
 *   allocated #GList of #ClutterActor<!-- -->s. Use g_list_free() when
 *   done.
 *
 * Since: 1.10
 */
GList *
clutter_actor_get_children (ClutterActor *self)
{
  ClutterActor *iter;
  GList *res;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  /* we walk the list backward so that we can use prepend(),
   * which is O(1)
   */
  for (iter = self->priv->last_child, res = NULL;
       iter != NULL;
       iter = iter->priv->prev_sibling)
    {
      res = g_list_prepend (res, iter);
    }

  return res;
}

/*< private >
 * insert_child_at_depth:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 *
 * Inserts @child inside the list of children held by @self, using
 * the depth as the insertion criteria.
 *
 * This sadly makes the insertion not O(1), but we can keep the
 * list sorted so that the painters algorithm we use for painting
 * the children will work correctly.
 */
static void
insert_child_at_depth (ClutterActor *self,
                       ClutterActor *child,
                       gpointer      dummy G_GNUC_UNUSED)
{
  ClutterActor *iter;
  float child_depth;

  child->priv->parent = self;

  child_depth =
    _clutter_actor_get_transform_info_or_defaults (child)->depth;

  /* special-case the first child */
  if (self->priv->n_children == 0)
    {
      self->priv->first_child = child;
      self->priv->last_child = child;

      child->priv->next_sibling = NULL;
      child->priv->prev_sibling = NULL;

      return;
    }

  /* Find the right place to insert the child so that it will still be
     sorted and the child will be after all of the actors at the same
     dept */
  for (iter = self->priv->first_child;
       iter != NULL;
       iter = iter->priv->next_sibling)
    {
      float iter_depth;

      iter_depth =
        _clutter_actor_get_transform_info_or_defaults (iter)->depth;

      if (iter_depth > child_depth)
        break;
    }

  if (iter != NULL)
    {
      ClutterActor *tmp = iter->priv->prev_sibling;

      if (tmp != NULL)
        tmp->priv->next_sibling = child;

      /* Insert the node before the found one */
      child->priv->prev_sibling = iter->priv->prev_sibling;
      child->priv->next_sibling = iter;
      iter->priv->prev_sibling = child;
    }
  else
    {
      ClutterActor *tmp = self->priv->last_child;

      if (tmp != NULL)
        tmp->priv->next_sibling = child;

      /* insert the node at the end of the list */
      child->priv->prev_sibling = self->priv->last_child;
      child->priv->next_sibling = NULL;
    }

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

static void
insert_child_at_index (ClutterActor *self,
                       ClutterActor *child,
                       gpointer      data_)
{
  gint index_ = GPOINTER_TO_INT (data_);

  child->priv->parent = self;

  if (index_ == 0)
    {
      ClutterActor *tmp = self->priv->first_child;

      if (tmp != NULL)
        tmp->priv->prev_sibling = child;

      child->priv->prev_sibling = NULL;
      child->priv->next_sibling = tmp;
    }
  else if (index_ < 0 || index_ >= self->priv->n_children)
    {
      ClutterActor *tmp = self->priv->last_child;

      if (tmp != NULL)
        tmp->priv->next_sibling = child;

      child->priv->prev_sibling = tmp;
      child->priv->next_sibling = NULL;
    }
  else
    {
      ClutterActor *iter;
      int i;

      for (iter = self->priv->first_child, i = 0;
           iter != NULL;
           iter = iter->priv->next_sibling, i += 1)
        {
          if (index_ == i)
            {
              ClutterActor *tmp = iter->priv->prev_sibling;

              child->priv->prev_sibling = tmp;
              child->priv->next_sibling = iter;

              iter->priv->prev_sibling = child;

              if (tmp != NULL)
                tmp->priv->next_sibling = child;

              break;
            }
        }
    }

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

static void
insert_child_above (ClutterActor *self,
                    ClutterActor *child,
                    gpointer      data)
{
  ClutterActor *sibling = data;

  child->priv->parent = self;

  if (sibling == NULL)
    sibling = self->priv->last_child;

  child->priv->prev_sibling = sibling;

  if (sibling != NULL)
    {
      ClutterActor *tmp = sibling->priv->next_sibling;

      child->priv->next_sibling = tmp;

      if (tmp != NULL)
        tmp->priv->prev_sibling = child;

      sibling->priv->next_sibling = child;
    }
  else
    child->priv->next_sibling = NULL;

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

static void
insert_child_below (ClutterActor *self,
                    ClutterActor *child,
                    gpointer      data)
{
  ClutterActor *sibling = data;

  child->priv->parent = self;

  if (sibling == NULL)
    sibling = self->priv->first_child;

  child->priv->next_sibling = sibling;

  if (sibling != NULL)
    {
      ClutterActor *tmp = sibling->priv->prev_sibling;

      child->priv->prev_sibling = tmp;

      if (tmp != NULL)
        tmp->priv->next_sibling = child;

      sibling->priv->prev_sibling = child;
    }
  else
    child->priv->prev_sibling = NULL;

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

typedef void (* ClutterActorAddChildFunc) (ClutterActor *parent,
                                           ClutterActor *child,
                                           gpointer      data);

typedef enum {
  ADD_CHILD_CREATE_META       = 1 << 0,
  ADD_CHILD_EMIT_PARENT_SET   = 1 << 1,
  ADD_CHILD_EMIT_ACTOR_ADDED  = 1 << 2,
  ADD_CHILD_CHECK_STATE       = 1 << 3,
  ADD_CHILD_NOTIFY_FIRST_LAST = 1 << 4,

  /* default flags for public API */
  ADD_CHILD_DEFAULT_FLAGS    = ADD_CHILD_CREATE_META |
                               ADD_CHILD_EMIT_PARENT_SET |
                               ADD_CHILD_EMIT_ACTOR_ADDED |
                               ADD_CHILD_CHECK_STATE |
                               ADD_CHILD_NOTIFY_FIRST_LAST,

  /* flags for legacy/deprecated API */
  ADD_CHILD_LEGACY_FLAGS     = ADD_CHILD_EMIT_PARENT_SET |
                               ADD_CHILD_CHECK_STATE |
                               ADD_CHILD_NOTIFY_FIRST_LAST
} ClutterActorAddChildFlags;

/*< private >
 * clutter_actor_add_child_internal:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 * @flags: control flags for actions
 * @add_func: delegate function
 * @data: (closure): data to pass to @add_func
 *
 * Adds @child to the list of children of @self.
 *
 * The actual insertion inside the list is delegated to @add_func: this
 * function will just set up the state, perform basic checks, and emit
 * signals.
 *
 * The @flags argument is used to perform additional operations.
 */
static inline void
clutter_actor_add_child_internal (ClutterActor              *self,
                                  ClutterActor              *child,
                                  ClutterActorAddChildFlags  flags,
                                  ClutterActorAddChildFunc   add_func,
                                  gpointer                   data)
{
  ClutterTextDirection text_dir;
  gboolean create_meta;
  gboolean emit_parent_set, emit_actor_added;
  gboolean check_state;
  gboolean notify_first_last;
  ClutterActor *old_first_child, *old_last_child;

  if (child->priv->parent != NULL)
    {
      g_warning ("The actor '%s' already has a parent, '%s'. You must "
                 "use clutter_actor_remove_child() first.",
                 _clutter_actor_get_debug_name (child),
                 _clutter_actor_get_debug_name (child->priv->parent));
      return;
    }

  if (CLUTTER_ACTOR_IS_TOPLEVEL (child))
    {
      g_warning ("The actor '%s' is a top-level actor, and cannot be "
                 "a child of another actor.",
                 _clutter_actor_get_debug_name (child));
      return;
    }

#if 0
  /* XXX - this check disallows calling methods that change the stacking
   * order within the destruction sequence, by triggering a critical
   * warning first, and leaving the actor in an undefined state, which
   * then ends up being caught by an assertion.
   *
   * the reproducible sequence is:
   *
   *   - actor gets destroyed;
   *   - another actor, linked to the first, will try to change the
   *     stacking order of the first actor;
   *   - changing the stacking order is a composite operation composed
   *     by the following steps:
   *     1. ref() the child;
   *     2. remove_child_internal(), which removes the reference;
   *     3. add_child_internal(), which adds a reference;
   *   - the state of the actor is not changed between (2) and (3), as
   *     it could be an expensive recomputation;
   *   - if (3) bails out, then the actor is in an undefined state, but
   *     still alive;
   *   - the destruction sequence terminates, but the actor is unparented
   *     while its state indicates being parented instead.
   *   - assertion failure.
   *
   * the obvious fix would be to decompose each set_child_*_sibling()
   * method into proper remove_child()/add_child(), with state validation;
   * this may cause excessive work, though, and trigger a cascade of other
   * bugs in code that assumes that a change in the stacking order is an
   * atomic operation.
   *
   * another potential fix is to just remove this check here, and let
   * code doing stacking order changes inside the destruction sequence
   * of an actor continue doing the work.
   *
   * the third fix is to silently bail out early from every
   * set_child_*_sibling() and set_child_at_index() method, and avoid
   * doing work.
   *
   * I have a preference for the second solution, since it involves the
   * least amount of work, and the least amount of code duplication.
   *
   * see bug: https://bugzilla.gnome.org/show_bug.cgi?id=670647
   */
  if (CLUTTER_ACTOR_IN_DESTRUCTION (child))
    {
      g_warning ("The actor '%s' is currently being destroyed, and "
                 "cannot be added as a child of another actor.",
                 _clutter_actor_get_debug_name (child));
      return;
    }
#endif

  create_meta = (flags & ADD_CHILD_CREATE_META) != 0;
  emit_parent_set = (flags & ADD_CHILD_EMIT_PARENT_SET) != 0;
  emit_actor_added = (flags & ADD_CHILD_EMIT_ACTOR_ADDED) != 0;
  check_state = (flags & ADD_CHILD_CHECK_STATE) != 0;
  notify_first_last = (flags & ADD_CHILD_NOTIFY_FIRST_LAST) != 0;

  old_first_child = self->priv->first_child;
  old_last_child = self->priv->last_child;

  g_object_freeze_notify (G_OBJECT (self));

  if (create_meta)
    clutter_container_create_child_meta (CLUTTER_CONTAINER (self), child);

  g_object_ref_sink (child);
  child->priv->parent = NULL;
  child->priv->next_sibling = NULL;
  child->priv->prev_sibling = NULL;

  /* delegate the actual insertion */
  add_func (self, child, data);

  g_assert (child->priv->parent == self);

  self->priv->n_children += 1;

  self->priv->age += 1;

  /* if push_internal() has been called then we automatically set
   * the flag on the actor
   */
  if (self->priv->internal_child)
    CLUTTER_SET_PRIVATE_FLAGS (child, CLUTTER_INTERNAL_CHILD);

  /* clutter_actor_reparent() will emit ::parent-set for us */
  if (emit_parent_set && !CLUTTER_ACTOR_IN_REPARENT (child))
    g_signal_emit (child, actor_signals[PARENT_SET], 0, NULL);

  if (check_state)
    {
      /* If parent is mapped or realized, we need to also be mapped or
       * realized once we're inside the parent.
       */
      clutter_actor_update_map_state (child, MAP_STATE_CHECK);

      /* propagate the parent's text direction to the child */
      text_dir = clutter_actor_get_text_direction (self);
      clutter_actor_set_text_direction (child, text_dir);
    }

  if (child->priv->show_on_set_parent)
    clutter_actor_show (child);

  if (CLUTTER_ACTOR_IS_MAPPED (child))
    clutter_actor_queue_redraw (child);

  if (emit_actor_added)
    g_signal_emit_by_name (self, "actor-added", child);

  if (notify_first_last)
    {
      if (old_first_child != self->priv->first_child)
        g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_FIRST_CHILD]);

      if (old_last_child != self->priv->last_child)
        g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_LAST_CHILD]);
    }

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_add_child:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 *
 * Adds @child to the children of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling clutter_actor_remove_child().
 *
 * This function will take into consideration the #ClutterActor:depth
 * of @child, and will keep the list of children sorted.
 *
 * This function will emit the #ClutterContainer::actor-added signal
 * on @self.
 *
 * Since: 1.10
 */
void
clutter_actor_add_child (ClutterActor *self,
                         ClutterActor *child)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child->priv->parent == NULL);

  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_DEFAULT_FLAGS,
                                    insert_child_at_depth,
                                    NULL);
}

/**
 * clutter_actor_insert_child_at_index:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 * @index_: the index
 *
 * Inserts @child into the list of children of @self, using the
 * given @index_. If @index_ is greater than the number of children
 * in @self, or is less than 0, then the new child is added at the end.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling clutter_actor_remove_child().
 *
 * This function will not take into consideration the #ClutterActor:depth
 * of @child.
 *
 * This function will emit the #ClutterContainer::actor-added signal
 * on @self.
 *
 * Since: 1.10
 */
void
clutter_actor_insert_child_at_index (ClutterActor *self,
                                     ClutterActor *child,
                                     gint          index_)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child->priv->parent == NULL);

  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_DEFAULT_FLAGS,
                                    insert_child_at_index,
                                    GINT_TO_POINTER (index_));
}

/**
 * clutter_actor_insert_child_above:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 * @sibling: (allow-none): a child of @self, or %NULL
 *
 * Inserts @child into the list of children of @self, above another
 * child of @self or, if @sibling is %NULL, above all the children
 * of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling clutter_actor_remove_child().
 *
 * This function will not take into consideration the #ClutterActor:depth
 * of @child.
 *
 * This function will emit the #ClutterContainer::actor-added signal
 * on @self.
 *
 * Since: 1.10
 */
void
clutter_actor_insert_child_above (ClutterActor *self,
                                  ClutterActor *child,
                                  ClutterActor *sibling)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child != sibling);
  g_return_if_fail (child->priv->parent == NULL);
  g_return_if_fail (sibling == NULL ||
                    (CLUTTER_IS_ACTOR (sibling) &&
                     sibling->priv->parent == self));

  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_DEFAULT_FLAGS,
                                    insert_child_above,
                                    sibling);
}

/**
 * clutter_actor_insert_child_below:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 * @sibling: (allow-none): a child of @self, or %NULL
 *
 * Inserts @child into the list of children of @self, below another
 * child of @self or, if @sibling is %NULL, below all the children
 * of @self.
 *
 * This function will acquire a reference on @child that will only
 * be released when calling clutter_actor_remove_child().
 *
 * This function will not take into consideration the #ClutterActor:depth
 * of @child.
 *
 * This function will emit the #ClutterContainer::actor-added signal
 * on @self.
 *
 * Since: 1.10
 */
void
clutter_actor_insert_child_below (ClutterActor *self,
                                  ClutterActor *child,
                                  ClutterActor *sibling)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child != sibling);
  g_return_if_fail (child->priv->parent == NULL);
  g_return_if_fail (sibling == NULL ||
                    (CLUTTER_IS_ACTOR (sibling) &&
                     sibling->priv->parent == self));

  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_DEFAULT_FLAGS,
                                    insert_child_below,
                                    sibling);
}

/**
 * clutter_actor_get_parent:
 * @self: A #ClutterActor
 *
 * Retrieves the parent of @self.
 *
 * Return Value: (transfer none): The #ClutterActor parent, or %NULL
 *  if no parent is set
 */
ClutterActor *
clutter_actor_get_parent (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return self->priv->parent;
}

/**
 * clutter_actor_get_paint_visibility:
 * @self: A #ClutterActor
 *
 * Retrieves the 'paint' visibility of an actor recursively checking for non
 * visible parents.
 *
 * This is by definition the same as %CLUTTER_ACTOR_IS_MAPPED.
 *
 * Return Value: %TRUE if the actor is visibile and will be painted.
 *
 * Since: 0.8.4
 */
gboolean
clutter_actor_get_paint_visibility (ClutterActor *actor)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (actor), FALSE);

  return CLUTTER_ACTOR_IS_MAPPED (actor);
}

/**
 * clutter_actor_remove_child:
 * @self: a #ClutterActor
 * @child: a #ClutterActor
 *
 * Removes @child from the children of @self.
 *
 * This function will release the reference added by
 * clutter_actor_add_child(), so if you want to keep using @child
 * you will have to acquire a referenced on it before calling this
 * function.
 *
 * This function will emit the #ClutterContainer::actor-removed
 * signal on @self.
 *
 * Since: 1.10
 */
void
clutter_actor_remove_child (ClutterActor *self,
                            ClutterActor *child)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (self != child);
  g_return_if_fail (child->priv->parent != NULL);
  g_return_if_fail (child->priv->parent == self);

  clutter_actor_remove_child_internal (self, child,
                                       REMOVE_CHILD_DEFAULT_FLAGS);
}

/**
 * clutter_actor_remove_all_children:
 * @self: a #ClutterActor
 *
 * Removes all children of @self.
 *
 * This function releases the reference added by inserting a child actor
 * in the list of children of @self.
 *
 * If the reference count of a child drops to zero, the child will be
 * destroyed. If you want to ensure the destruction of all the children
 * of @self, use clutter_actor_destroy_all_children().
 *
 * Since: 1.10
 */
void
clutter_actor_remove_all_children (ClutterActor *self)
{
  ClutterActorIter iter;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (self->priv->n_children == 0)
    return;

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_iter_init (&iter, self);
  while (clutter_actor_iter_next (&iter, NULL))
    clutter_actor_iter_remove (&iter);

  g_object_thaw_notify (G_OBJECT (self));

  /* sanity check */
  g_assert (self->priv->first_child == NULL);
  g_assert (self->priv->last_child == NULL);
  g_assert (self->priv->n_children == 0);
}

/**
 * clutter_actor_destroy_all_children:
 * @self: a #ClutterActor
 *
 * Destroys all children of @self.
 *
 * This function releases the reference added by inserting a child
 * actor in the list of children of @self, and ensures that the
 * #ClutterActor::destroy signal is emitted on each child of the
 * actor.
 *
 * By default, #ClutterActor will emit the #ClutterActor::destroy signal
 * when its reference count drops to 0; the default handler of the
 * #ClutterActor::destroy signal will destroy all the children of an
 * actor. This function ensures that all children are destroyed, instead
 * of just removed from @self, unlike clutter_actor_remove_all_children()
 * which will merely release the reference and remove each child.
 *
 * Unless you acquired an additional reference on each child of @self
 * prior to calling clutter_actor_remove_all_children() and want to reuse
 * the actors, you should use clutter_actor_destroy_all_children() in
 * order to make sure that children are destroyed and signal handlers
 * are disconnected even in cases where circular references prevent this
 * from automatically happening through reference counting alone.
 *
 * Since: 1.10
 */
void
clutter_actor_destroy_all_children (ClutterActor *self)
{
  ClutterActorIter iter;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (self->priv->n_children == 0)
    return;

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_iter_init (&iter, self);
  while (clutter_actor_iter_next (&iter, NULL))
    clutter_actor_iter_destroy (&iter);

  g_object_thaw_notify (G_OBJECT (self));

  /* sanity check */
  g_assert (self->priv->first_child == NULL);
  g_assert (self->priv->last_child == NULL);
  g_assert (self->priv->n_children == 0);
}

typedef struct _InsertBetweenData {
  ClutterActor *prev_sibling;
  ClutterActor *next_sibling;
} InsertBetweenData;

static void
insert_child_between (ClutterActor *self,
                      ClutterActor *child,
                      gpointer      data_)
{
  InsertBetweenData *data = data_;
  ClutterActor *prev_sibling = data->prev_sibling;
  ClutterActor *next_sibling = data->next_sibling;

  child->priv->parent = self;
  child->priv->prev_sibling = prev_sibling;
  child->priv->next_sibling = next_sibling;

  if (prev_sibling != NULL)
    prev_sibling->priv->next_sibling = child;

  if (next_sibling != NULL)
    next_sibling->priv->prev_sibling = child;

  if (child->priv->prev_sibling == NULL)
    self->priv->first_child = child;

  if (child->priv->next_sibling == NULL)
    self->priv->last_child = child;
}

/**
 * clutter_actor_replace_child:
 * @self: a #ClutterActor
 * @old_child: the child of @self to replace
 * @new_child: the #ClutterActor to replace @old_child
 *
 * Replaces @old_child with @new_child in the list of children of @self.
 *
 * Since: 1.10
 */
void
clutter_actor_replace_child (ClutterActor *self,
                             ClutterActor *old_child,
                             ClutterActor *new_child)
{
  ClutterActor *prev_sibling, *next_sibling;
  InsertBetweenData clos;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (old_child));
  g_return_if_fail (old_child->priv->parent == self);
  g_return_if_fail (CLUTTER_IS_ACTOR (new_child));
  g_return_if_fail (old_child != new_child);
  g_return_if_fail (new_child != self);
  g_return_if_fail (new_child->priv->parent == NULL);

  prev_sibling = old_child->priv->prev_sibling;
  next_sibling = old_child->priv->next_sibling;
  clutter_actor_remove_child_internal (self, old_child,
                                       REMOVE_CHILD_DEFAULT_FLAGS);

  clos.prev_sibling = prev_sibling;
  clos.next_sibling = next_sibling;
  clutter_actor_add_child_internal (self, new_child,
                                    ADD_CHILD_DEFAULT_FLAGS,
                                    insert_child_between,
                                    &clos);
}

/**
 * clutter_actor_contains:
 * @self: A #ClutterActor
 * @descendant: A #ClutterActor, possibly contained in @self
 *
 * Determines if @descendant is contained inside @self (either as an
 * immediate child, or as a deeper descendant). If @self and
 * @descendant point to the same actor then it will also return %TRUE.
 *
 * Return value: whether @descendent is contained within @self
 *
 * Since: 1.4
 */
gboolean
clutter_actor_contains (ClutterActor *self,
			ClutterActor *descendant)
{
  ClutterActor *actor;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);
  g_return_val_if_fail (CLUTTER_IS_ACTOR (descendant), FALSE);

  for (actor = descendant; actor; actor = actor->priv->parent)
    if (actor == self)
      return TRUE;

  return FALSE;
}

/**
 * clutter_actor_set_child_above_sibling:
 * @self: a #ClutterActor
 * @child: a #ClutterActor child of @self
 * @sibling: (allow-none): a #ClutterActor child of @self, or %NULL
 *
 * Sets @child to be above @sibling in the list of children of @self.
 *
 * If @sibling is %NULL, @child will be the new last child of @self.
 *
 * This function is logically equivalent to removing @child and using
 * clutter_actor_insert_child_above(), but it will not emit signals
 * or change state on @child.
 *
 * Since: 1.10
 */
void
clutter_actor_set_child_above_sibling (ClutterActor *self,
                                       ClutterActor *child,
                                       ClutterActor *sibling)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (child->priv->parent == self);
  g_return_if_fail (child != sibling);
  g_return_if_fail (sibling == NULL || CLUTTER_IS_ACTOR (sibling));

  if (sibling != NULL)
    g_return_if_fail (sibling->priv->parent == self);

  /* we don't want to change the state of child, or emit signals, or
   * regenerate ChildMeta instances here, but we still want to follow
   * the correct sequence of steps encoded in remove_child() and
   * add_child(), so that correctness is ensured, and we only go
   * through one known code path.
   */
  g_object_ref (child);
  clutter_actor_remove_child_internal (self, child, 0);
  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_NOTIFY_FIRST_LAST,
                                    insert_child_above,
                                    sibling);

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_set_child_below_sibling:
 * @self: a #ClutterActor
 * @child: a #ClutterActor child of @self
 * @sibling: (allow-none): a #ClutterActor child of @self, or %NULL
 *
 * Sets @child to be below @sibling in the list of children of @self.
 *
 * If @sibling is %NULL, @child will be the new first child of @self.
 *
 * This function is logically equivalent to removing @self and using
 * clutter_actor_insert_child_below(), but it will not emit signals
 * or change state on @child.
 *
 * Since: 1.10
 */
void
clutter_actor_set_child_below_sibling (ClutterActor *self,
                                       ClutterActor *child,
                                       ClutterActor *sibling)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (child->priv->parent == self);
  g_return_if_fail (child != sibling);
  g_return_if_fail (sibling == NULL || CLUTTER_IS_ACTOR (sibling));

  if (sibling != NULL)
    g_return_if_fail (sibling->priv->parent == self);

  /* see the comment in set_child_above_sibling() */
  g_object_ref (child);
  clutter_actor_remove_child_internal (self, child, 0);
  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_NOTIFY_FIRST_LAST,
                                    insert_child_below,
                                    sibling);

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_set_child_at_index:
 * @self: a #ClutterActor
 * @child: a #ClutterActor child of @self
 * @index_: the new index for @child
 *
 * Changes the index of @child in the list of children of @self.
 *
 * This function is logically equivalent to removing @child and
 * calling clutter_actor_insert_child_at_index(), but it will not
 * emit signals or change state on @child.
 *
 * Since: 1.10
 */
void
clutter_actor_set_child_at_index (ClutterActor *self,
                                  ClutterActor *child,
                                  gint          index_)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (child));
  g_return_if_fail (child->priv->parent == self);
  g_return_if_fail (index_ <= self->priv->n_children);

  g_object_ref (child);
  clutter_actor_remove_child_internal (self, child, 0);
  clutter_actor_add_child_internal (self, child,
                                    ADD_CHILD_NOTIFY_FIRST_LAST,
                                    insert_child_at_index,
                                    GINT_TO_POINTER (index_));

  clutter_actor_queue_redraw (self);
}

/*
 * Event handling
 */

/**
 * clutter_actor_event:
 * @actor: a #ClutterActor
 * @event: a #ClutterEvent
 * @capture: TRUE if event in in capture phase, FALSE otherwise.
 *
 * This function is used to emit an event on the main stage.
 * You should rarely need to use this function, except for
 * synthetising events.
 *
 * Return value: the return value from the signal emission: %TRUE
 *   if the actor handled the event, or %FALSE if the event was
 *   not handled
 *
 * Since: 0.6
 */
gboolean
clutter_actor_event (ClutterActor *actor,
                     ClutterEvent *event,
		     gboolean      capture)
{
  gboolean retval = FALSE;
  gint signal_num = -1;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (actor), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  g_object_ref (actor);

  if (capture)
    {
      g_signal_emit (actor, actor_signals[CAPTURED_EVENT], 0,
		     event,
                     &retval);
      goto out;
    }

  g_signal_emit (actor, actor_signals[EVENT], 0, event, &retval);

  if (!retval)
    {
      switch (event->type)
	{
	case CLUTTER_NOTHING:
	  break;
	case CLUTTER_BUTTON_PRESS:
	  signal_num = BUTTON_PRESS_EVENT;
	  break;
	case CLUTTER_BUTTON_RELEASE:
	  signal_num = BUTTON_RELEASE_EVENT;
	  break;
	case CLUTTER_SCROLL:
	  signal_num = SCROLL_EVENT;
	  break;
	case CLUTTER_KEY_PRESS:
	  signal_num = KEY_PRESS_EVENT;
	  break;
	case CLUTTER_KEY_RELEASE:
	  signal_num = KEY_RELEASE_EVENT;
	  break;
	case CLUTTER_MOTION:
	  signal_num = MOTION_EVENT;
	  break;
	case CLUTTER_ENTER:
	  signal_num = ENTER_EVENT;
	  break;
	case CLUTTER_LEAVE:
	  signal_num = LEAVE_EVENT;
	  break;
	case CLUTTER_DELETE:
	case CLUTTER_DESTROY_NOTIFY:
	case CLUTTER_CLIENT_MESSAGE:
	default:
	  signal_num = -1;
	  break;
	}

      if (signal_num != -1)
	g_signal_emit (actor, actor_signals[signal_num], 0,
		       event, &retval);
    }

out:
  g_object_unref (actor);

  return retval;
}

/**
 * clutter_actor_set_reactive:
 * @actor: a #ClutterActor
 * @reactive: whether the actor should be reactive to events
 *
 * Sets @actor as reactive. Reactive actors will receive events.
 *
 * Since: 0.6
 */
void
clutter_actor_set_reactive (ClutterActor *actor,
                            gboolean      reactive)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (actor));

  if (reactive == CLUTTER_ACTOR_IS_REACTIVE (actor))
    return;

  if (reactive)
    CLUTTER_ACTOR_SET_FLAGS (actor, CLUTTER_ACTOR_REACTIVE);
  else
    CLUTTER_ACTOR_UNSET_FLAGS (actor, CLUTTER_ACTOR_REACTIVE);

  g_object_notify_by_pspec (G_OBJECT (actor), obj_props[PROP_REACTIVE]);
}

/**
 * clutter_actor_get_reactive:
 * @actor: a #ClutterActor
 *
 * Checks whether @actor is marked as reactive.
 *
 * Return value: %TRUE if the actor is reactive
 *
 * Since: 0.6
 */
gboolean
clutter_actor_get_reactive (ClutterActor *actor)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (actor), FALSE);

  return CLUTTER_ACTOR_IS_REACTIVE (actor) ? TRUE : FALSE;
}

/**
 * clutter_actor_get_anchor_point:
 * @self: a #ClutterActor
 * @anchor_x: (out): return location for the X coordinate of the anchor point
 * @anchor_y: (out): return location for the Y coordinate of the anchor point
 *
 * Gets the current anchor point of the @actor in pixels.
 *
 * Since: 0.6
 */
void
clutter_actor_get_anchor_point (ClutterActor *self,
				gfloat       *anchor_x,
                                gfloat       *anchor_y)
{
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_transform_info_or_defaults (self);
  clutter_anchor_coord_get_units (self, &info->anchor,
                                  anchor_x,
                                  anchor_y,
                                  NULL);
}

/**
 * clutter_actor_set_anchor_point:
 * @self: a #ClutterActor
 * @anchor_x: X coordinate of the anchor point
 * @anchor_y: Y coordinate of the anchor point
 *
 * Sets an anchor point for @self. The anchor point is a point in the
 * coordinate space of an actor to which the actor position within its
 * parent is relative; the default is (0, 0), i.e. the top-left corner
 * of the actor.
 *
 * Since: 0.6
 */
void
clutter_actor_set_anchor_point (ClutterActor *self,
                                gfloat        anchor_x,
                                gfloat        anchor_y)
{
  ClutterTransformInfo *info;
  ClutterActorPrivate *priv;
  gboolean changed = FALSE;
  gfloat old_anchor_x, old_anchor_y;
  GObject *obj;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  obj = G_OBJECT (self);
  priv = self->priv;
  info = _clutter_actor_get_transform_info (self);

  g_object_freeze_notify (obj);

  clutter_anchor_coord_get_units (self, &info->anchor,
                                  &old_anchor_x,
                                  &old_anchor_y,
                                  NULL);

  if (info->anchor.is_fractional)
    g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_GRAVITY]);

  if (old_anchor_x != anchor_x)
    {
      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_X]);
      changed = TRUE;
    }

  if (old_anchor_y != anchor_y)
    {
      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_Y]);
      changed = TRUE;
    }

  clutter_anchor_coord_set_units (&info->anchor, anchor_x, anchor_y, 0);

  if (changed)
    {
      priv->transform_valid = FALSE;
      clutter_actor_queue_redraw (self);
    }

  g_object_thaw_notify (obj);
}

/**
 * clutter_actor_get_anchor_point_gravity:
 * @self: a #ClutterActor
 *
 * Retrieves the anchor position expressed as a #ClutterGravity. If
 * the anchor point was specified using pixels or units this will
 * return %CLUTTER_GRAVITY_NONE.
 *
 * Return value: the #ClutterGravity used by the anchor point
 *
 * Since: 1.0
 */
ClutterGravity
clutter_actor_get_anchor_point_gravity (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), CLUTTER_GRAVITY_NONE);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  return clutter_anchor_coord_get_gravity (&info->anchor);
}

/**
 * clutter_actor_move_anchor_point:
 * @self: a #ClutterActor
 * @anchor_x: X coordinate of the anchor point
 * @anchor_y: Y coordinate of the anchor point
 *
 * Sets an anchor point for the actor, and adjusts the actor postion so that
 * the relative position of the actor toward its parent remains the same.
 *
 * Since: 0.6
 */
void
clutter_actor_move_anchor_point (ClutterActor *self,
                                 gfloat        anchor_x,
                                 gfloat        anchor_y)
{
  gfloat old_anchor_x, old_anchor_y;
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_transform_info (self);
  clutter_anchor_coord_get_units (self, &info->anchor,
                                  &old_anchor_x,
                                  &old_anchor_y,
                                  NULL);

  g_object_freeze_notify (G_OBJECT (self));

  clutter_actor_set_anchor_point (self, anchor_x, anchor_y);

  if (self->priv->position_set)
    clutter_actor_move_by (self,
                           anchor_x - old_anchor_x,
                           anchor_y - old_anchor_y);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_move_anchor_point_from_gravity:
 * @self: a #ClutterActor
 * @gravity: #ClutterGravity.
 *
 * Sets an anchor point on the actor based on the given gravity, adjusting the
 * actor postion so that its relative position within its parent remains
 * unchanged.
 *
 * Since version 1.0 the anchor point will be stored as a gravity so
 * that if the actor changes size then the anchor point will move. For
 * example, if you set the anchor point to %CLUTTER_GRAVITY_SOUTH_EAST
 * and later double the size of the actor, the anchor point will move
 * to the bottom right.
 *
 * Since: 0.6
 */
void
clutter_actor_move_anchor_point_from_gravity (ClutterActor   *self,
					      ClutterGravity  gravity)
{
  gfloat old_anchor_x, old_anchor_y, new_anchor_x, new_anchor_y;
  const ClutterTransformInfo *info;
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  priv = self->priv;
  info = _clutter_actor_get_transform_info (self);

  g_object_freeze_notify (G_OBJECT (self));

  clutter_anchor_coord_get_units (self, &info->anchor,
                                  &old_anchor_x,
                                  &old_anchor_y,
                                  NULL);
  clutter_actor_set_anchor_point_from_gravity (self, gravity);
  clutter_anchor_coord_get_units (self, &info->anchor,
                                  &new_anchor_x,
                                  &new_anchor_y,
                                  NULL);

  if (priv->position_set)
    clutter_actor_move_by (self,
                           new_anchor_x - old_anchor_x,
                           new_anchor_y - old_anchor_y);

  g_object_thaw_notify (G_OBJECT (self));
}

/**
 * clutter_actor_set_anchor_point_from_gravity:
 * @self: a #ClutterActor
 * @gravity: #ClutterGravity.
 *
 * Sets an anchor point on the actor, based on the given gravity (this is a
 * convenience function wrapping clutter_actor_set_anchor_point()).
 *
 * Since version 1.0 the anchor point will be stored as a gravity so
 * that if the actor changes size then the anchor point will move. For
 * example, if you set the anchor point to %CLUTTER_GRAVITY_SOUTH_EAST
 * and later double the size of the actor, the anchor point will move
 * to the bottom right.
 *
 * Since: 0.6
 */
void
clutter_actor_set_anchor_point_from_gravity (ClutterActor   *self,
					     ClutterGravity  gravity)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (gravity == CLUTTER_GRAVITY_NONE)
    clutter_actor_set_anchor_point (self, 0, 0);
  else
    {
      GObject *obj = G_OBJECT (self);
      ClutterTransformInfo *info;

      g_object_freeze_notify (obj);

      info = _clutter_actor_get_transform_info (self);
      clutter_anchor_coord_set_gravity (&info->anchor, gravity);

      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_GRAVITY]);
      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_X]);
      g_object_notify_by_pspec (obj, obj_props[PROP_ANCHOR_Y]);

      self->priv->transform_valid = FALSE;

      clutter_actor_queue_redraw (self);

      g_object_thaw_notify (obj);
    }
}

static void
clutter_container_iface_init (ClutterContainerIface *iface)
{
  /* we don't override anything, as ClutterContainer already has a default
   * implementation that we can use, and which calls into our own API.
   */
}

typedef enum
{
  PARSE_X,
  PARSE_Y,
  PARSE_WIDTH,
  PARSE_HEIGHT,
  PARSE_ANCHOR_X,
  PARSE_ANCHOR_Y
} ParseDimension;

static gfloat
parse_units (ClutterActor   *self,
             ParseDimension  dimension,
             JsonNode       *node)
{
  GValue value = G_VALUE_INIT;
  gfloat retval = 0;

  if (JSON_NODE_TYPE (node) != JSON_NODE_VALUE)
    return 0;

  json_node_get_value (node, &value);

  if (G_VALUE_HOLDS (&value, G_TYPE_INT64))
    {
      retval = (gfloat) g_value_get_int64 (&value);
    }
  else if (G_VALUE_HOLDS (&value, G_TYPE_DOUBLE))
    {
      retval = g_value_get_double (&value);
    }
  else if (G_VALUE_HOLDS (&value, G_TYPE_STRING))
    {
      ClutterUnits units;
      gboolean res;

      res = clutter_units_from_string (&units, g_value_get_string (&value));
      if (res)
        retval = clutter_units_to_pixels (&units);
      else
        {
          g_warning ("Invalid value '%s': integers, strings or floating point "
                     "values can be used for the x, y, width and height "
                     "properties. Valid modifiers for strings are 'px', 'mm', "
                     "'pt' and 'em'.",
                     g_value_get_string (&value));
          retval = 0;
        }
    }
  else
    {
      g_warning ("Invalid value of type '%s': integers, strings of floating "
                 "point values can be used for the x, y, width, height "
                 "anchor-x and anchor-y properties.",
                 g_type_name (G_VALUE_TYPE (&value)));
    }

  g_value_unset (&value);

  return retval;
}

typedef struct {
  ClutterRotateAxis axis;

  gdouble angle;

  gfloat center_x;
  gfloat center_y;
  gfloat center_z;
} RotationInfo;

static inline gboolean
parse_rotation_array (ClutterActor *actor,
                      JsonArray    *array,
                      RotationInfo *info)
{
  JsonNode *element;

  if (json_array_get_length (array) != 2)
    return FALSE;

  /* angle */
  element = json_array_get_element (array, 0);
  if (JSON_NODE_TYPE (element) == JSON_NODE_VALUE)
    info->angle = json_node_get_double (element);
  else
    return FALSE;

  /* center */
  element = json_array_get_element (array, 1);
  if (JSON_NODE_TYPE (element) == JSON_NODE_ARRAY)
    {
      JsonArray *center = json_node_get_array (element);

      if (json_array_get_length (center) != 2)
        return FALSE;

      switch (info->axis)
        {
        case CLUTTER_X_AXIS:
          info->center_y = parse_units (actor, PARSE_Y,
                                        json_array_get_element (center, 0));
          info->center_z = parse_units (actor, PARSE_Y,
                                        json_array_get_element (center, 1));
          return TRUE;

        case CLUTTER_Y_AXIS:
          info->center_x = parse_units (actor, PARSE_X,
                                        json_array_get_element (center, 0));
          info->center_z = parse_units (actor, PARSE_X,
                                        json_array_get_element (center, 1));
          return TRUE;

        case CLUTTER_Z_AXIS:
          info->center_x = parse_units (actor, PARSE_X,
                                        json_array_get_element (center, 0));
          info->center_y = parse_units (actor, PARSE_Y,
                                        json_array_get_element (center, 1));
          return TRUE;
        }
    }

  return FALSE;
}

static gboolean
parse_rotation (ClutterActor *actor,
                JsonNode     *node,
                RotationInfo *info)
{
  JsonArray *array;
  guint len, i;
  gboolean retval = FALSE;

  if (JSON_NODE_TYPE (node) != JSON_NODE_ARRAY)
    {
      g_warning ("Invalid node of type '%s' found, expecting an array",
                 json_node_type_name (node));
      return FALSE;
    }

  array = json_node_get_array (node);
  len = json_array_get_length (array);

  for (i = 0; i < len; i++)
    {
      JsonNode *element = json_array_get_element (array, i);
      JsonObject *object;
      JsonNode *member;

      if (JSON_NODE_TYPE (element) != JSON_NODE_OBJECT)
        {
          g_warning ("Invalid node of type '%s' found, expecting an object",
                     json_node_type_name (element));
          return FALSE;
        }

      object = json_node_get_object (element);

      if (json_object_has_member (object, "x-axis"))
        {
          member = json_object_get_member (object, "x-axis");

          info->axis = CLUTTER_X_AXIS;

          if (JSON_NODE_TYPE (member) == JSON_NODE_VALUE)
            {
              info->angle = json_node_get_double (member);
              retval = TRUE;
            }
          else if (JSON_NODE_TYPE (member) == JSON_NODE_ARRAY)
            retval = parse_rotation_array (actor,
                                           json_node_get_array (member),
                                           info);
          else
            retval = FALSE;
        }
      else if (json_object_has_member (object, "y-axis"))
        {
          member = json_object_get_member (object, "y-axis");

          info->axis = CLUTTER_Y_AXIS;

          if (JSON_NODE_TYPE (member) == JSON_NODE_VALUE)
            {
              info->angle = json_node_get_double (member);
              retval = TRUE;
            }
          else if (JSON_NODE_TYPE (member) == JSON_NODE_ARRAY)
            retval = parse_rotation_array (actor,
                                           json_node_get_array (member),
                                           info);
          else
            retval = FALSE;
        }
      else if (json_object_has_member (object, "z-axis"))
        {
          member = json_object_get_member (object, "z-axis");

          info->axis = CLUTTER_Z_AXIS;

          if (JSON_NODE_TYPE (member) == JSON_NODE_VALUE)
            {
              info->angle = json_node_get_double (member);
              retval = TRUE;
            }
          else if (JSON_NODE_TYPE (member) == JSON_NODE_ARRAY)
            retval = parse_rotation_array (actor,
                                           json_node_get_array (member),
                                           info);
          else
            retval = FALSE;
        }
    }

  return retval;
}

static GSList *
parse_actor_metas (ClutterScript *script,
                   ClutterActor  *actor,
                   JsonNode      *node)
{
  GList *elements, *l;
  GSList *retval = NULL;

  if (!JSON_NODE_HOLDS_ARRAY (node))
    return NULL;

  elements = json_array_get_elements (json_node_get_array (node));

  for (l = elements; l != NULL; l = l->next)
    {
      JsonNode *element = l->data;
      const gchar *id_ = _clutter_script_get_id_from_node (element);
      GObject *meta;

      if (id_ == NULL || *id_ == '\0')
        continue;

      meta = clutter_script_get_object (script, id_);
      if (meta == NULL)
        continue;

      retval = g_slist_prepend (retval, meta);
    }

  g_list_free (elements);

  return g_slist_reverse (retval);
}

static gboolean
clutter_actor_parse_custom_node (ClutterScriptable *scriptable,
                                 ClutterScript     *script,
                                 GValue            *value,
                                 const gchar       *name,
                                 JsonNode          *node)
{
  ClutterActor *actor = CLUTTER_ACTOR (scriptable);
  gboolean retval = FALSE;

  if ((name[0] == 'x' && name[1] == '\0') ||
      (name[0] == 'y' && name[1] == '\0') ||
      (strcmp (name, "width") == 0) ||
      (strcmp (name, "height") == 0) ||
      (strcmp (name, "anchor_x") == 0) ||
      (strcmp (name, "anchor_y") == 0))
    {
      ParseDimension dimension;
      gfloat units;

      if (name[0] == 'x')
        dimension = PARSE_X;
      else if (name[0] == 'y')
        dimension = PARSE_Y;
      else if (name[0] == 'w')
        dimension = PARSE_WIDTH;
      else if (name[0] == 'h')
        dimension = PARSE_HEIGHT;
      else if (name[0] == 'a' && name[7] == 'x')
        dimension = PARSE_ANCHOR_X;
      else if (name[0] == 'a' && name[7] == 'y')
        dimension = PARSE_ANCHOR_Y;
      else
        return FALSE;

      units = parse_units (actor, dimension, node);

      /* convert back to pixels: all properties are pixel-based */
      g_value_init (value, G_TYPE_FLOAT);
      g_value_set_float (value, units);

      retval = TRUE;
    }
  else if (strcmp (name, "rotation") == 0)
    {
      RotationInfo *info;

      info = g_slice_new0 (RotationInfo);
      retval = parse_rotation (actor, node, info);

      if (retval)
        {
          g_value_init (value, G_TYPE_POINTER);
          g_value_set_pointer (value, info);
        }
      else
        g_slice_free (RotationInfo, info);
    }
  else if (strcmp (name, "effects") == 0)
    {
      GSList *l;

      l = parse_actor_metas (script, actor, node);

      g_value_init (value, G_TYPE_POINTER);
      g_value_set_pointer (value, l);

      retval = TRUE;
    }

  return retval;
}

static void
clutter_actor_set_custom_property (ClutterScriptable *scriptable,
                                   ClutterScript     *script,
                                   const gchar       *name,
                                   const GValue      *value)
{
  ClutterActor *actor = CLUTTER_ACTOR (scriptable);

#ifdef CLUTTER_ENABLE_DEBUG
  if (G_UNLIKELY (CLUTTER_HAS_DEBUG (SCRIPT)))
    {
      gchar *tmp = g_strdup_value_contents (value);

      CLUTTER_NOTE (SCRIPT,
                    "in ClutterActor::set_custom_property('%s') = %s",
                    name,
                    tmp);

      g_free (tmp);
    }
#endif /* CLUTTER_ENABLE_DEBUG */

  if (strcmp (name, "rotation") == 0)
    {
      RotationInfo *info;

      if (!G_VALUE_HOLDS (value, G_TYPE_POINTER))
        return;

      info = g_value_get_pointer (value);

      clutter_actor_set_rotation (actor,
                                  info->axis, info->angle,
                                  info->center_x,
                                  info->center_y,
                                  info->center_z);

      g_slice_free (RotationInfo, info);

      return;
    }

  if (strcmp (name, "effects") == 0)
    {
      GSList *metas, *l;

      if (!G_VALUE_HOLDS (value, G_TYPE_POINTER))
        return;

      metas = g_value_get_pointer (value);
      for (l = metas; l != NULL; l = l->next)
        clutter_actor_add_effect (actor, l->data);

      g_slist_free (metas);

      return;
    }

  g_object_set_property (G_OBJECT (scriptable), name, value);
}

static void
clutter_scriptable_iface_init (ClutterScriptableIface *iface)
{
  iface->parse_custom_node = clutter_actor_parse_custom_node;
  iface->set_custom_property = clutter_actor_set_custom_property;
}

static ClutterActorMeta *
get_meta_from_animation_property (ClutterActor  *actor,
                                  const gchar   *name,
                                  gchar        **name_p)
{
  ClutterActorPrivate *priv = actor->priv;
  ClutterActorMeta *meta = NULL;
  gchar **tokens;

  /* if this is not a special property, fall through */
  if (name[0] != '@')
    return NULL;

  /* detect the properties named using the following spec:
   *
   *   @<section>.<meta-name>.<property-name>
   *
   * where <section> can be one of the following:
   *
   *   - actions
   *   - constraints
   *   - effects
   *
   * and <meta-name> is the name set on a specific ActorMeta
   */

  tokens = g_strsplit (name + 1, ".", -1);
  if (tokens == NULL || g_strv_length (tokens) != 3)
    {
      CLUTTER_NOTE (ANIMATION, "Invalid property name '%s'",
                    name + 1);
      g_strfreev (tokens);
      return NULL;
    }

  if (strcmp (tokens[0], "effects") == 0)
    meta = _clutter_meta_group_get_meta (priv->effects, tokens[1]);

  if (name_p != NULL)
    *name_p = g_strdup (tokens[2]);

  CLUTTER_NOTE (ANIMATION,
                "Looking for property '%s' of object '%s' in section '%s'",
                tokens[2],
                tokens[1],
                tokens[0]);

  g_strfreev (tokens);

  return meta;
}

static GParamSpec *
clutter_actor_find_property (ClutterAnimatable *animatable,
                             const gchar       *property_name)
{
  ClutterActorMeta *meta = NULL;
  GObjectClass *klass = NULL;
  GParamSpec *pspec = NULL;
  gchar *p_name = NULL;

  meta = get_meta_from_animation_property (CLUTTER_ACTOR (animatable),
                                           property_name,
                                           &p_name);

  if (meta != NULL)
    {
      klass = G_OBJECT_GET_CLASS (meta);

      pspec = g_object_class_find_property (klass, p_name);
    }
  else
    {
      klass = G_OBJECT_GET_CLASS (animatable);

      pspec = g_object_class_find_property (klass, property_name);
    }

  g_free (p_name);

  return pspec;
}

static void
clutter_actor_get_initial_state (ClutterAnimatable *animatable,
                                 const gchar       *property_name,
                                 GValue            *initial)
{
  ClutterActorMeta *meta = NULL;
  gchar *p_name = NULL;

  meta = get_meta_from_animation_property (CLUTTER_ACTOR (animatable),
                                           property_name,
                                           &p_name);

  if (meta != NULL)
    g_object_get_property (G_OBJECT (meta), p_name, initial);
  else
    g_object_get_property (G_OBJECT (animatable), property_name, initial);

  g_free (p_name);
}

/*
 * clutter_actor_set_animatable_property:
 * @actor: a #ClutterActor
 * @prop_id: the paramspec id
 * @value: the value to set
 * @pspec: the paramspec
 *
 * Sets values of animatable properties.
 *
 * This is a variant of clutter_actor_set_property() that gets called
 * by the #ClutterAnimatable implementation of #ClutterActor for the
 * properties with the %CLUTTER_PARAM_ANIMATABLE flag set on their
 * #GParamSpec.
 *
 * Unlike the implementation of #GObjectClass.set_property(), this
 * function will not update the interval if a transition involving an
 * animatable property is in progress - this avoids cycles with the
 * transition API calling the public API.
 */
static void
clutter_actor_set_animatable_property (ClutterActor *actor,
                                       guint         prop_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
  GObject *obj = G_OBJECT (actor);

  g_object_freeze_notify (obj);

  switch (prop_id)
    {
    case PROP_X:
      clutter_actor_set_x_internal (actor, g_value_get_float (value));
      break;

    case PROP_Y:
      clutter_actor_set_y_internal (actor, g_value_get_float (value));
      break;

    case PROP_WIDTH:
      clutter_actor_set_width_internal (actor, g_value_get_float (value));
      break;

    case PROP_HEIGHT:
      clutter_actor_set_height_internal (actor, g_value_get_float (value));
      break;

    case PROP_DEPTH:
      clutter_actor_set_depth_internal (actor, g_value_get_float (value));
      break;

    case PROP_OPACITY:
      clutter_actor_set_opacity_internal (actor, g_value_get_uint (value));
      break;

    case PROP_SCALE_X:
      clutter_actor_set_scale_factor_internal (actor,
                                               g_value_get_double (value),
                                               pspec);
      break;

    case PROP_SCALE_Y:
      clutter_actor_set_scale_factor_internal (actor,
                                               g_value_get_double (value),
                                               pspec);
      break;

    case PROP_ROTATION_ANGLE_X:
      clutter_actor_set_rotation_angle_internal (actor,
                                                 CLUTTER_X_AXIS,
                                                 g_value_get_double (value));
      break;

    case PROP_ROTATION_ANGLE_Y:
      clutter_actor_set_rotation_angle_internal (actor,
                                                 CLUTTER_Y_AXIS,
                                                 g_value_get_double (value));
      break;

    case PROP_ROTATION_ANGLE_Z:
      clutter_actor_set_rotation_angle_internal (actor,
                                                 CLUTTER_Z_AXIS,
                                                 g_value_get_double (value));
      break;

    case PROP_BACKGROUND_COLOR:
      clutter_actor_set_background_color_internal (actor,
                                                   (ClutterColor *) g_value_get_boxed (value));
      break;

    default:
      g_object_set_property (obj, pspec->name, value);
      break;
    }

  g_object_thaw_notify (obj);
}

static void
clutter_actor_set_final_state (ClutterAnimatable *animatable,
                               const gchar       *property_name,
                               const GValue      *final)
{
  ClutterActor *actor = CLUTTER_ACTOR (animatable);
  ClutterActorMeta *meta = NULL;
  gchar *p_name = NULL;

  meta = get_meta_from_animation_property (actor,
                                           property_name,
                                           &p_name);
  if (meta != NULL)
    g_object_set_property (G_OBJECT (meta), p_name, final);
  else
    {
      GObjectClass *obj_class = G_OBJECT_GET_CLASS (animatable);
      GParamSpec *pspec;

      pspec = g_object_class_find_property (obj_class, property_name);

      if ((pspec->flags & CLUTTER_PARAM_ANIMATABLE) != 0)
        {
          /* XXX - I'm going to the special hell for this */
          clutter_actor_set_animatable_property (actor, pspec->param_id, final, pspec);
        }
      else
        g_object_set_property (G_OBJECT (animatable), pspec->name, final);
    }

  g_free (p_name);
}

static void
clutter_animatable_iface_init (ClutterAnimatableIface *iface)
{
  iface->find_property = clutter_actor_find_property;
  iface->get_initial_state = clutter_actor_get_initial_state;
  iface->set_final_state = clutter_actor_set_final_state;
}

/**
 * clutter_actor_transform_stage_point:
 * @self: A #ClutterActor
 * @x: (in): x screen coordinate of the point to unproject
 * @y: (in): y screen coordinate of the point to unproject
 * @x_out: (out): return location for the unprojected x coordinance
 * @y_out: (out): return location for the unprojected y coordinance
 *
 * This function translates screen coordinates (@x, @y) to
 * coordinates relative to the actor. For example, it can be used to translate
 * screen events from global screen coordinates into actor-local coordinates.
 *
 * The conversion can fail, notably if the transform stack results in the
 * actor being projected on the screen as a mere line.
 *
 * The conversion should not be expected to be pixel-perfect due to the
 * nature of the operation. In general the error grows when the skewing
 * of the actor rectangle on screen increases.
 *
 * <note><para>This function can be computationally intensive.</para></note>
 *
 * <note><para>This function only works when the allocation is up-to-date,
 * i.e. inside of paint().</para></note>
 *
 * Return value: %TRUE if conversion was successful.
 *
 * Since: 0.6
 */
gboolean
clutter_actor_transform_stage_point (ClutterActor *self,
				     gfloat        x,
				     gfloat        y,
				     gfloat       *x_out,
				     gfloat       *y_out)
{
  ClutterVertex v[4];
  float ST[3][3];
  float RQ[3][3];
  int du, dv, xi, yi;
  float px, py;
  float xf, yf, wf, det;
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  /* This implementation is based on the quad -> quad projection algorithm
   * described by Paul Heckbert in:
   *
   *   http://www.cs.cmu.edu/~ph/texfund/texfund.pdf
   *
   * and the sample implementation at:
   *
   *   http://www.cs.cmu.edu/~ph/src/texfund/
   *
   * Our texture is a rectangle with origin [0, 0], so we are mapping from
   * quad to rectangle only, which significantly simplifies things; the
   * function calls have been unrolled, and most of the math is done in fixed
   * point.
   */

  clutter_actor_get_abs_allocation_vertices (self, v);

  /* Keeping these as ints simplifies the multiplication (no significant
   * loss of precision here).
   */
  du = (int) info->width;
  dv = (int) info->height;

  if (!du || !dv)
    return FALSE;

#define UX2FP(x)        (x)
#define DET2FP(a,b,c,d) (((a) * (d)) - ((b) * (c)))

  /* First, find mapping from unit uv square to xy quadrilateral; this
   * equivalent to the pmap_square_quad() functions in the sample
   * implementation, which we can simplify, since our target is always
   * a rectangle.
   */
  px = v[0].x - v[1].x + v[3].x - v[2].x;
  py = v[0].y - v[1].y + v[3].y - v[2].y;

  if (!px && !py)
    {
      /* affine transform */
      RQ[0][0] = UX2FP (v[1].x - v[0].x);
      RQ[1][0] = UX2FP (v[3].x - v[1].x);
      RQ[2][0] = UX2FP (v[0].x);
      RQ[0][1] = UX2FP (v[1].y - v[0].y);
      RQ[1][1] = UX2FP (v[3].y - v[1].y);
      RQ[2][1] = UX2FP (v[0].y);
      RQ[0][2] = 0;
      RQ[1][2] = 0;
      RQ[2][2] = 1.0;
    }
  else
    {
      /* projective transform */
      double dx1, dx2, dy1, dy2, del;

      dx1 = UX2FP (v[1].x - v[3].x);
      dx2 = UX2FP (v[2].x - v[3].x);
      dy1 = UX2FP (v[1].y - v[3].y);
      dy2 = UX2FP (v[2].y - v[3].y);

      del = DET2FP (dx1, dx2, dy1, dy2);
      if (!del)
	return FALSE;

      /*
       * The division here needs to be done in floating point for
       * precisions reasons.
       */
      RQ[0][2] = (DET2FP (UX2FP (px), dx2, UX2FP (py), dy2) / del);
      RQ[1][2] = (DET2FP (dx1, UX2FP (px), dy1, UX2FP (py)) / del);
      RQ[1][2] = (DET2FP (dx1, UX2FP (px), dy1, UX2FP (py)) / del);
      RQ[2][2] = 1.0;
      RQ[0][0] = UX2FP (v[1].x - v[0].x) + (RQ[0][2] * UX2FP (v[1].x));
      RQ[1][0] = UX2FP (v[2].x - v[0].x) + (RQ[1][2] * UX2FP (v[2].x));
      RQ[2][0] = UX2FP (v[0].x);
      RQ[0][1] = UX2FP (v[1].y - v[0].y) + (RQ[0][2] * UX2FP (v[1].y));
      RQ[1][1] = UX2FP (v[2].y - v[0].y) + (RQ[1][2] * UX2FP (v[2].y));
      RQ[2][1] = UX2FP (v[0].y);
    }

  /*
   * Now combine with transform from our rectangle (u0,v0,u1,v1) to unit
   * square. Since our rectangle is based at 0,0 we only need to scale.
   */
  RQ[0][0] /= du;
  RQ[1][0] /= dv;
  RQ[0][1] /= du;
  RQ[1][1] /= dv;
  RQ[0][2] /= du;
  RQ[1][2] /= dv;

  /*
   * Now RQ is transform from uv rectangle to xy quadrilateral; we need an
   * inverse of that.
   */
  ST[0][0] = DET2FP (RQ[1][1], RQ[1][2], RQ[2][1], RQ[2][2]);
  ST[1][0] = DET2FP (RQ[1][2], RQ[1][0], RQ[2][2], RQ[2][0]);
  ST[2][0] = DET2FP (RQ[1][0], RQ[1][1], RQ[2][0], RQ[2][1]);
  ST[0][1] = DET2FP (RQ[2][1], RQ[2][2], RQ[0][1], RQ[0][2]);
  ST[1][1] = DET2FP (RQ[2][2], RQ[2][0], RQ[0][2], RQ[0][0]);
  ST[2][1] = DET2FP (RQ[2][0], RQ[2][1], RQ[0][0], RQ[0][1]);
  ST[0][2] = DET2FP (RQ[0][1], RQ[0][2], RQ[1][1], RQ[1][2]);
  ST[1][2] = DET2FP (RQ[0][2], RQ[0][0], RQ[1][2], RQ[1][0]);
  ST[2][2] = DET2FP (RQ[0][0], RQ[0][1], RQ[1][0], RQ[1][1]);

  /*
   * Check the resulting matrix is OK.
   */
  det = (RQ[0][0] * ST[0][0])
      + (RQ[0][1] * ST[0][1])
      + (RQ[0][2] * ST[0][2]);
  if (!det)
    return FALSE;

  /*
   * Now transform our point with the ST matrix; the notional w
   * coordinate is 1, hence the last part is simply added.
   */
  xi = (int) x;
  yi = (int) y;

  xf = xi * ST[0][0] + yi * ST[1][0] + ST[2][0];
  yf = xi * ST[0][1] + yi * ST[1][1] + ST[2][1];
  wf = xi * ST[0][2] + yi * ST[1][2] + ST[2][2];

  if (x_out)
    *x_out = xf / wf;

  if (y_out)
    *y_out = yf / wf;

#undef UX2FP
#undef DET2FP

  return TRUE;
}

/*
 * ClutterGeometry
 */

static ClutterGeometry*
clutter_geometry_copy (const ClutterGeometry *geometry)
{
  return g_slice_dup (ClutterGeometry, geometry);
}

static void
clutter_geometry_free (ClutterGeometry *geometry)
{
  if (G_LIKELY (geometry != NULL))
    g_slice_free (ClutterGeometry, geometry);
}

/**
 * clutter_geometry_union:
 * @geometry_a: a #ClutterGeometry
 * @geometry_b: another #ClutterGeometry
 * @result: (out): location to store the result
 *
 * Find the union of two rectangles represented as #ClutterGeometry.
 *
 * Since: 1.4
 */
void
clutter_geometry_union (const ClutterGeometry *geometry_a,
                        const ClutterGeometry *geometry_b,
                        ClutterGeometry       *result)
{
  /* We don't try to handle rectangles that can't be represented
   * as a signed integer box */
  gint x_1 = MIN (geometry_a->x, geometry_b->x);
  gint y_1 = MIN (geometry_a->y, geometry_b->y);
  gint x_2 = MAX (geometry_a->x + (gint)geometry_a->width,
                  geometry_b->x + (gint)geometry_b->width);
  gint y_2 = MAX (geometry_a->y + (gint)geometry_a->height,
                  geometry_b->y + (gint)geometry_b->height);
  result->x = x_1;
  result->y = y_1;
  result->width = x_2 - x_1;
  result->height = y_2 - y_1;
}

/**
 * clutter_geometry_intersects:
 * @geometry0: The first geometry to test
 * @geometry1: The second geometry to test
 *
 * Determines if @geometry0 and geometry1 intersect returning %TRUE if
 * they do else %FALSE.
 *
 * Return value: %TRUE of @geometry0 and geometry1 intersect else
 * %FALSE.
 *
 * Since: 1.4
 */
gboolean
clutter_geometry_intersects (const ClutterGeometry *geometry0,
                             const ClutterGeometry *geometry1)
{
  if (geometry1->x >= (geometry0->x + (gint)geometry0->width) ||
      geometry1->y >= (geometry0->y + (gint)geometry0->height) ||
      (geometry1->x + (gint)geometry1->width) <= geometry0->x ||
      (geometry1->y + (gint)geometry1->height) <= geometry0->y)
    return FALSE;
  else
    return TRUE;
}

static gboolean
clutter_geometry_progress (const GValue *a,
                           const GValue *b,
                           gdouble       progress,
                           GValue       *retval)
{
  const ClutterGeometry *a_geom = g_value_get_boxed (a);
  const ClutterGeometry *b_geom = g_value_get_boxed (b);
  ClutterGeometry res = { 0, };
  gint a_width = a_geom->width;
  gint b_width = b_geom->width;
  gint a_height = a_geom->height;
  gint b_height = b_geom->height;

  res.x = a_geom->x + (b_geom->x - a_geom->x) * progress;
  res.y = a_geom->y + (b_geom->y - a_geom->y) * progress;

  res.width = a_width + (b_width - a_width) * progress;
  res.height = a_height + (b_height - a_height) * progress;

  g_value_set_boxed (retval, &res);

  return TRUE;
}

G_DEFINE_BOXED_TYPE_WITH_CODE (ClutterGeometry, clutter_geometry,
                               clutter_geometry_copy,
                               clutter_geometry_free,
                               CLUTTER_REGISTER_INTERVAL_PROGRESS (clutter_geometry_progress));

/*
 * ClutterVertices
 */

/**
 * clutter_vertex_new:
 * @x: X coordinate
 * @y: Y coordinate
 * @z: Z coordinate
 *
 * Creates a new #ClutterVertex for the point in 3D space
 * identified by the 3 coordinates @x, @y, @z
 *
 * Return value: the newly allocate #ClutterVertex. Use
 *   clutter_vertex_free() to free the resources
 *
 * Since: 1.0
 */
ClutterVertex *
clutter_vertex_new (gfloat x,
                    gfloat y,
                    gfloat z)
{
  ClutterVertex *vertex;

  vertex = g_slice_new (ClutterVertex);
  clutter_vertex_init (vertex, x, y, z);

  return vertex;
}

/**
 * clutter_vertex_init:
 * @vertex: a #ClutterVertex
 * @x: X coordinate
 * @y: Y coordinate
 * @z: Z coordinate
 *
 * Initializes @vertex with the given coordinates.
 *
 * Since: 1.10
 */
void
clutter_vertex_init (ClutterVertex *vertex,
                     gfloat         x,
                     gfloat         y,
                     gfloat         z)
{
  g_return_if_fail (vertex != NULL);

  vertex->x = x;
  vertex->y = y;
  vertex->z = z;
}

/**
 * clutter_vertex_copy:
 * @vertex: a #ClutterVertex
 *
 * Copies @vertex
 *
 * Return value: a newly allocated copy of #ClutterVertex. Use
 *   clutter_vertex_free() to free the allocated resources
 *
 * Since: 1.0
 */
ClutterVertex *
clutter_vertex_copy (const ClutterVertex *vertex)
{
  if (G_LIKELY (vertex != NULL))
    return g_slice_dup (ClutterVertex, vertex);

  return NULL;
}

/**
 * clutter_vertex_free:
 * @vertex: a #ClutterVertex
 *
 * Frees a #ClutterVertex allocated using clutter_vertex_copy()
 *
 * Since: 1.0
 */
void
clutter_vertex_free (ClutterVertex *vertex)
{
  if (G_UNLIKELY (vertex != NULL))
    g_slice_free (ClutterVertex, vertex);
}

/**
 * clutter_vertex_equal:
 * @vertex_a: a #ClutterVertex
 * @vertex_b: a #ClutterVertex
 *
 * Compares @vertex_a and @vertex_b for equality
 *
 * Return value: %TRUE if the passed #ClutterVertex are equal
 *
 * Since: 1.0
 */
gboolean
clutter_vertex_equal (const ClutterVertex *vertex_a,
                      const ClutterVertex *vertex_b)
{
  g_return_val_if_fail (vertex_a != NULL && vertex_b != NULL, FALSE);

  if (vertex_a == vertex_b)
    return TRUE;

  return vertex_a->x == vertex_b->x &&
         vertex_a->y == vertex_b->y &&
         vertex_a->z == vertex_b->z;
}

static gboolean
clutter_vertex_progress (const GValue *a,
                         const GValue *b,
                         gdouble       progress,
                         GValue       *retval)
{
  const ClutterVertex *av = g_value_get_boxed (a);
  const ClutterVertex *bv = g_value_get_boxed (b);
  ClutterVertex res = { 0, };

  res.x = av->x + (bv->x - av->x) * progress;
  res.y = av->y + (bv->y - av->y) * progress;
  res.z = av->z + (bv->z - av->z) * progress;

  g_value_set_boxed (retval, &res);

  return TRUE;
}

G_DEFINE_BOXED_TYPE_WITH_CODE (ClutterVertex, clutter_vertex,
                               clutter_vertex_copy,
                               clutter_vertex_free,
                               CLUTTER_REGISTER_INTERVAL_PROGRESS (clutter_vertex_progress));

/**
 * clutter_actor_is_rotated:
 * @self: a #ClutterActor
 *
 * Checks whether any rotation is applied to the actor.
 *
 * Return value: %TRUE if the actor is rotated.
 *
 * Since: 0.6
 */
gboolean
clutter_actor_is_rotated (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  if (info->rx_angle || info->ry_angle || info->rz_angle)
    return TRUE;

  return FALSE;
}

/**
 * clutter_actor_is_scaled:
 * @self: a #ClutterActor
 *
 * Checks whether the actor is scaled in either dimension.
 *
 * Return value: %TRUE if the actor is scaled.
 *
 * Since: 0.6
 */
gboolean
clutter_actor_is_scaled (ClutterActor *self)
{
  const ClutterTransformInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  if (info->scale_x != 1.0 || info->scale_y != 1.0)
    return TRUE;

  return FALSE;
}

ClutterActor *
_clutter_actor_get_stage_internal (ClutterActor *actor)
{
  while (actor && !CLUTTER_ACTOR_IS_TOPLEVEL (actor))
    actor = actor->priv->parent;

  return actor;
}

/**
 * clutter_actor_get_stage:
 * @actor: a #ClutterActor
 *
 * Retrieves the #ClutterStage where @actor is contained.
 *
 * Return value: (transfer none) (type Clutter.Stage): the stage
 *   containing the actor, or %NULL
 *
 * Since: 0.8
 */
ClutterActor *
clutter_actor_get_stage (ClutterActor *actor)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (actor), NULL);

  return _clutter_actor_get_stage_internal (actor);
}

/**
 * clutter_actor_grab_key_focus:
 * @self: a #ClutterActor
 *
 * Sets the key focus of the #ClutterStage including @self
 * to this #ClutterActor.
 *
 * Since: 1.0
 */
void
clutter_actor_grab_key_focus (ClutterActor *self)
{
  ClutterActor *stage;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  stage = _clutter_actor_get_stage_internal (self);
  if (stage != NULL)
    clutter_stage_set_key_focus (CLUTTER_STAGE (stage), self);
}

/**
 * clutter_actor_get_pango_context:
 * @self: a #ClutterActor
 *
 * Retrieves the #PangoContext for @self. The actor's #PangoContext
 * is already configured using the appropriate font map, resolution
 * and font options.
 *
 * Unlike clutter_actor_create_pango_context(), this context is owend
 * by the #ClutterActor and it will be updated each time the options
 * stored by the #ClutterBackend change.
 *
 * You can use the returned #PangoContext to create a #PangoLayout
 * and render text using cogl_pango_render_layout() to reuse the
 * glyphs cache also used by Clutter.
 *
 * Return value: (transfer none): the #PangoContext for a #ClutterActor.
 *   The returned #PangoContext is owned by the actor and should not be
 *   unreferenced by the application code
 *
 * Since: 1.0
 */
PangoContext *
clutter_actor_get_pango_context (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  priv = self->priv;

  if (priv->pango_context != NULL)
    return priv->pango_context;

  priv->pango_context = _clutter_context_get_pango_context ();
  g_object_ref (priv->pango_context);

  return priv->pango_context;
}

/**
 * clutter_actor_create_pango_context:
 * @self: a #ClutterActor
 *
 * Creates a #PangoContext for the given actor. The #PangoContext
 * is already configured using the appropriate font map, resolution
 * and font options.
 *
 * See also clutter_actor_get_pango_context().
 *
 * Return value: (transfer full): the newly created #PangoContext.
 *   Use g_object_unref() on the returned value to deallocate its
 *   resources
 *
 * Since: 1.0
 */
PangoContext *
clutter_actor_create_pango_context (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return _clutter_context_create_pango_context ();
}

/**
 * clutter_actor_create_pango_layout:
 * @self: a #ClutterActor
 * @text: (allow-none) the text to set on the #PangoLayout, or %NULL
 *
 * Creates a new #PangoLayout from the same #PangoContext used
 * by the #ClutterActor. The #PangoLayout is already configured
 * with the font map, resolution and font options, and the
 * given @text.
 *
 * If you want to keep around a #PangoLayout created by this
 * function you will have to connect to the #ClutterBackend::font-changed
 * and #ClutterBackend::resolution-changed signals, and call
 * pango_layout_context_changed() in response to them.
 *
 * Return value: (transfer full): the newly created #PangoLayout.
 *   Use g_object_unref() when done
 *
 * Since: 1.0
 */
PangoLayout *
clutter_actor_create_pango_layout (ClutterActor *self,
                                   const gchar  *text)
{
  PangoContext *context;
  PangoLayout *layout;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  context = clutter_actor_get_pango_context (self);
  layout = pango_layout_new (context);

  if (text)
    pango_layout_set_text (layout, text, -1);

  return layout;
}

/* Allows overriding the calculated paint opacity. Used by ClutterClone and
 * ClutterOffscreenEffect.
 */
void
_clutter_actor_set_opacity_override (ClutterActor *self,
                                     gint          opacity)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  self->priv->opacity_override = opacity;
}

gint
_clutter_actor_get_opacity_override (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), -1);

  return self->priv->opacity_override;
}

/* Allows you to disable applying the actors model view transform during
 * a paint. Used by ClutterClone. */
void
_clutter_actor_set_enable_model_view_transform (ClutterActor *self,
                                                gboolean      enable)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  self->priv->enable_model_view_transform = enable;
}

void
_clutter_actor_set_enable_paint_unmapped (ClutterActor *self,
                                          gboolean      enable)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  priv = self->priv;

  priv->enable_paint_unmapped = enable;

  if (priv->enable_paint_unmapped)
    {
      /* Make sure that the parents of the widget are realized first;
       * otherwise checks in clutter_actor_update_map_state() will
       * fail.
       */
      clutter_actor_realize (self);

      clutter_actor_update_map_state (self, MAP_STATE_MAKE_MAPPED);
    }
  else
    {
      clutter_actor_update_map_state (self, MAP_STATE_MAKE_UNMAPPED);
    }
}

static void
clutter_anchor_coord_get_units (ClutterActor      *self,
                                const AnchorCoord *coord,
                                gfloat            *x,
                                gfloat            *y,
                                gfloat            *z)
{
  if (coord->is_fractional)
    {
      gfloat actor_width, actor_height;

      clutter_actor_get_size (self, &actor_width, &actor_height);

      if (x)
        *x = actor_width * coord->v.fraction.x;

      if (y)
        *y = actor_height * coord->v.fraction.y;

      if (z)
        *z = 0;
    }
  else
    {
      if (x)
        *x = coord->v.units.x;

      if (y)
        *y = coord->v.units.y;

      if (z)
        *z = coord->v.units.z;
    }
}

static void
clutter_anchor_coord_set_units (AnchorCoord *coord,
                                gfloat       x,
                                gfloat       y,
                                gfloat       z)
{
  coord->is_fractional = FALSE;
  coord->v.units.x = x;
  coord->v.units.y = y;
  coord->v.units.z = z;
}

static ClutterGravity
clutter_anchor_coord_get_gravity (const AnchorCoord *coord)
{
  if (coord->is_fractional)
    {
      if (coord->v.fraction.x == 0.0)
        {
          if (coord->v.fraction.y == 0.0)
            return CLUTTER_GRAVITY_NORTH_WEST;
          else if (coord->v.fraction.y == 0.5)
            return CLUTTER_GRAVITY_WEST;
          else if (coord->v.fraction.y == 1.0)
            return CLUTTER_GRAVITY_SOUTH_WEST;
          else
            return CLUTTER_GRAVITY_NONE;
        }
      else if (coord->v.fraction.x == 0.5)
        {
          if (coord->v.fraction.y == 0.0)
            return CLUTTER_GRAVITY_NORTH;
          else if (coord->v.fraction.y == 0.5)
            return CLUTTER_GRAVITY_CENTER;
          else if (coord->v.fraction.y == 1.0)
            return CLUTTER_GRAVITY_SOUTH;
          else
            return CLUTTER_GRAVITY_NONE;
        }
      else if (coord->v.fraction.x == 1.0)
        {
          if (coord->v.fraction.y == 0.0)
            return CLUTTER_GRAVITY_NORTH_EAST;
          else if (coord->v.fraction.y == 0.5)
            return CLUTTER_GRAVITY_EAST;
          else if (coord->v.fraction.y == 1.0)
            return CLUTTER_GRAVITY_SOUTH_EAST;
          else
            return CLUTTER_GRAVITY_NONE;
        }
      else
        return CLUTTER_GRAVITY_NONE;
    }
  else
    return CLUTTER_GRAVITY_NONE;
}

static void
clutter_anchor_coord_set_gravity (AnchorCoord    *coord,
                                  ClutterGravity  gravity)
{
  switch (gravity)
    {
    case CLUTTER_GRAVITY_NORTH:
      coord->v.fraction.x = 0.5;
      coord->v.fraction.y = 0.0;
      break;

    case CLUTTER_GRAVITY_NORTH_EAST:
      coord->v.fraction.x = 1.0;
      coord->v.fraction.y = 0.0;
      break;

    case CLUTTER_GRAVITY_EAST:
      coord->v.fraction.x = 1.0;
      coord->v.fraction.y = 0.5;
      break;

    case CLUTTER_GRAVITY_SOUTH_EAST:
      coord->v.fraction.x = 1.0;
      coord->v.fraction.y = 1.0;
      break;

    case CLUTTER_GRAVITY_SOUTH:
      coord->v.fraction.x = 0.5;
      coord->v.fraction.y = 1.0;
      break;

    case CLUTTER_GRAVITY_SOUTH_WEST:
      coord->v.fraction.x = 0.0;
      coord->v.fraction.y = 1.0;
      break;

    case CLUTTER_GRAVITY_WEST:
      coord->v.fraction.x = 0.0;
      coord->v.fraction.y = 0.5;
      break;

    case CLUTTER_GRAVITY_NORTH_WEST:
      coord->v.fraction.x = 0.0;
      coord->v.fraction.y = 0.0;
      break;

    case CLUTTER_GRAVITY_CENTER:
      coord->v.fraction.x = 0.5;
      coord->v.fraction.y = 0.5;
      break;

    default:
      coord->v.fraction.x = 0.0;
      coord->v.fraction.y = 0.0;
      break;
    }

  coord->is_fractional = TRUE;
}

static gboolean
clutter_anchor_coord_is_zero (const AnchorCoord *coord)
{
  if (coord->is_fractional)
    return coord->v.fraction.x == 0.0 && coord->v.fraction.y == 0.0;
  else
    return (coord->v.units.x == 0.0
            && coord->v.units.y == 0.0
            && coord->v.units.z == 0.0);
}

/**
 * clutter_actor_get_flags:
 * @self: a #ClutterActor
 *
 * Retrieves the flags set on @self
 *
 * Return value: a bitwise or of #ClutterActorFlags or 0
 *
 * Since: 1.0
 */
ClutterActorFlags
clutter_actor_get_flags (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  return self->flags;
}

/**
 * clutter_actor_set_flags:
 * @self: a #ClutterActor
 * @flags: the flags to set
 *
 * Sets @flags on @self
 *
 * This function will emit notifications for the changed properties
 *
 * Since: 1.0
 */
void
clutter_actor_set_flags (ClutterActor      *self,
                         ClutterActorFlags  flags)
{
  ClutterActorFlags old_flags;
  GObject *obj;
  gboolean was_reactive_set, reactive_set;
  gboolean was_realized_set, realized_set;
  gboolean was_mapped_set, mapped_set;
  gboolean was_visible_set, visible_set;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (self->flags == flags)
    return;

  obj = G_OBJECT (self);
  g_object_ref (obj);
  g_object_freeze_notify (obj);

  old_flags = self->flags;

  was_reactive_set = ((old_flags & CLUTTER_ACTOR_REACTIVE) != 0);
  was_realized_set = ((old_flags & CLUTTER_ACTOR_REALIZED) != 0);
  was_mapped_set   = ((old_flags & CLUTTER_ACTOR_MAPPED)   != 0);
  was_visible_set  = ((old_flags & CLUTTER_ACTOR_VISIBLE)  != 0);

  self->flags |= flags;

  reactive_set = ((self->flags & CLUTTER_ACTOR_REACTIVE) != 0);
  realized_set = ((self->flags & CLUTTER_ACTOR_REALIZED) != 0);
  mapped_set   = ((self->flags & CLUTTER_ACTOR_MAPPED)   != 0);
  visible_set  = ((self->flags & CLUTTER_ACTOR_VISIBLE)  != 0);

  if (reactive_set != was_reactive_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_REACTIVE]);

  if (realized_set != was_realized_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_REALIZED]);

  if (mapped_set != was_mapped_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_MAPPED]);

  if (visible_set != was_visible_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_VISIBLE]);

  g_object_thaw_notify (obj);
  g_object_unref (obj);
}

/**
 * clutter_actor_unset_flags:
 * @self: a #ClutterActor
 * @flags: the flags to unset
 *
 * Unsets @flags on @self
 *
 * This function will emit notifications for the changed properties
 *
 * Since: 1.0
 */
void
clutter_actor_unset_flags (ClutterActor      *self,
                           ClutterActorFlags  flags)
{
  ClutterActorFlags old_flags;
  GObject *obj;
  gboolean was_reactive_set, reactive_set;
  gboolean was_realized_set, realized_set;
  gboolean was_mapped_set, mapped_set;
  gboolean was_visible_set, visible_set;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  obj = G_OBJECT (self);
  g_object_freeze_notify (obj);

  old_flags = self->flags;

  was_reactive_set = ((old_flags & CLUTTER_ACTOR_REACTIVE) != 0);
  was_realized_set = ((old_flags & CLUTTER_ACTOR_REALIZED) != 0);
  was_mapped_set   = ((old_flags & CLUTTER_ACTOR_MAPPED)   != 0);
  was_visible_set  = ((old_flags & CLUTTER_ACTOR_VISIBLE)  != 0);

  self->flags &= ~flags;

  if (self->flags == old_flags)
    return;

  reactive_set = ((self->flags & CLUTTER_ACTOR_REACTIVE) != 0);
  realized_set = ((self->flags & CLUTTER_ACTOR_REALIZED) != 0);
  mapped_set   = ((self->flags & CLUTTER_ACTOR_MAPPED)   != 0);
  visible_set  = ((self->flags & CLUTTER_ACTOR_VISIBLE)  != 0);

  if (reactive_set != was_reactive_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_REACTIVE]);

  if (realized_set != was_realized_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_REALIZED]);

  if (mapped_set != was_mapped_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_MAPPED]);

  if (visible_set != was_visible_set)
    g_object_notify_by_pspec (obj, obj_props[PROP_VISIBLE]);

  g_object_thaw_notify (obj);
}

/**
 * clutter_actor_get_transformation_matrix:
 * @self: a #ClutterActor
 * @matrix: (out caller-allocates): the return location for a #CoglMatrix
 *
 * Retrieves the transformations applied to @self relative to its
 * parent.
 *
 * Since: 1.0
 */
void
clutter_actor_get_transformation_matrix (ClutterActor *self,
                                         CoglMatrix   *matrix)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  cogl_matrix_init_identity (matrix);

  _clutter_actor_apply_modelview_transform (self, matrix);
}

void
_clutter_actor_set_in_clone_paint (ClutterActor *self,
                                   gboolean      is_in_clone_paint)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  self->priv->in_clone_paint = is_in_clone_paint;
}

/**
 * clutter_actor_is_in_clone_paint:
 * @self: a #ClutterActor
 *
 * Checks whether @self is being currently painted by a #ClutterClone
 *
 * This function is useful only inside the ::paint virtual function
 * implementations or within handlers for the #ClutterActor::paint
 * signal
 *
 * This function should not be used by applications
 *
 * Return value: %TRUE if the #ClutterActor is currently being painted
 *   by a #ClutterClone, and %FALSE otherwise
 *
 * Since: 1.0
 */
gboolean
clutter_actor_is_in_clone_paint (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  return self->priv->in_clone_paint;
}

static gboolean
set_direction_recursive (ClutterActor *actor,
                         gpointer      user_data)
{
  ClutterTextDirection text_dir = GPOINTER_TO_INT (user_data);

  clutter_actor_set_text_direction (actor, text_dir);

  return TRUE;
}

/**
 * clutter_actor_set_text_direction:
 * @self: a #ClutterActor
 * @text_dir: the text direction for @self
 *
 * Sets the #ClutterTextDirection for an actor
 *
 * The passed text direction must not be %CLUTTER_TEXT_DIRECTION_DEFAULT
 *
 * If @self implements #ClutterContainer then this function will recurse
 * inside all the children of @self (including the internal ones).
 *
 * Composite actors not implementing #ClutterContainer, or actors requiring
 * special handling when the text direction changes, should connect to
 * the #GObject::notify signal for the #ClutterActor:text-direction property
 *
 * Since: 1.2
 */
void
clutter_actor_set_text_direction (ClutterActor         *self,
                                  ClutterTextDirection  text_dir)
{
  ClutterActorPrivate *priv;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (text_dir != CLUTTER_TEXT_DIRECTION_DEFAULT);

  priv = self->priv;

  if (priv->text_direction != text_dir)
    {
      priv->text_direction = text_dir;

      /* we need to emit the notify::text-direction first, so that
       * the sub-classes can catch that and do specific handling of
       * the text direction; see clutter_text_direction_changed_cb()
       * inside clutter-text.c
       */
      g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_TEXT_DIRECTION]);

      _clutter_actor_foreach_child (self, set_direction_recursive,
                                    GINT_TO_POINTER (text_dir));

      clutter_actor_queue_redraw (self);
    }
}

void
_clutter_actor_set_has_pointer (ClutterActor *self,
                                gboolean      has_pointer)
{
  ClutterActorPrivate *priv = self->priv;

  if (priv->has_pointer != has_pointer)
    {
      priv->has_pointer = has_pointer;

      g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_HAS_POINTER]);
    }
}

/**
 * clutter_actor_get_text_direction:
 * @self: a #ClutterActor
 *
 * Retrieves the value set using clutter_actor_set_text_direction()
 *
 * If no text direction has been previously set, the default text
 * direction, as returned by clutter_get_default_text_direction(), will
 * be returned instead
 *
 * Return value: the #ClutterTextDirection for the actor
 *
 * Since: 1.2
 */
ClutterTextDirection
clutter_actor_get_text_direction (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self),
                        CLUTTER_TEXT_DIRECTION_LTR);

  priv = self->priv;

  /* if no direction has been set yet use the default */
  if (priv->text_direction == CLUTTER_TEXT_DIRECTION_DEFAULT)
    priv->text_direction = clutter_get_default_text_direction ();

  return priv->text_direction;
}

/**
 * clutter_actor_has_pointer:
 * @self: a #ClutterActor
 *
 * Checks whether an actor contains the pointer of a
 * #ClutterInputDevice
 *
 * Return value: %TRUE if the actor contains the pointer, and
 *   %FALSE otherwise
 *
 * Since: 1.2
 */
gboolean
clutter_actor_has_pointer (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  return self->priv->has_pointer;
}

/* XXX: This is a workaround for not being able to break the ABI of
 * the QUEUE_REDRAW signal. It is an out-of-band argument.  See
 * clutter_actor_queue_clipped_redraw() for details.
 */
ClutterPaintVolume *
_clutter_actor_get_queue_redraw_clip (ClutterActor *self)
{
  return g_object_get_data (G_OBJECT (self),
                            "-clutter-actor-queue-redraw-clip");
}

void
_clutter_actor_set_queue_redraw_clip (ClutterActor       *self,
                                      ClutterPaintVolume *clip)
{
  g_object_set_data (G_OBJECT (self),
                     "-clutter-actor-queue-redraw-clip",
                     clip);
}

/**
 * clutter_actor_add_effect:
 * @self: a #ClutterActor
 * @effect: a #ClutterEffect
 *
 * Adds @effect to the list of #ClutterEffect<!-- -->s applied to @self
 *
 * The #ClutterActor will hold a reference on the @effect until either
 * clutter_actor_remove_effect() or clutter_actor_clear_effects() is
 * called.
 *
 * Since: 1.4
 */
void
clutter_actor_add_effect (ClutterActor  *self,
                          ClutterEffect *effect)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_EFFECT (effect));

  _clutter_actor_add_effect_internal (self, effect);

  clutter_actor_queue_redraw (self);

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_EFFECT]);
}

/**
 * clutter_actor_add_effect_with_name:
 * @self: a #ClutterActor
 * @name: the name to set on the effect
 * @effect: a #ClutterEffect
 *
 * A convenience function for setting the name of a #ClutterEffect
 * while adding it to the list of effectss applied to @self
 *
 * This function is the logical equivalent of:
 *
 * |[
 *   clutter_actor_meta_set_name (CLUTTER_ACTOR_META (effect), name);
 *   clutter_actor_add_effect (self, effect);
 * ]|
 *
 * Since: 1.4
 */
void
clutter_actor_add_effect_with_name (ClutterActor  *self,
                                    const gchar   *name,
                                    ClutterEffect *effect)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (name != NULL);
  g_return_if_fail (CLUTTER_IS_EFFECT (effect));

  clutter_actor_meta_set_name (CLUTTER_ACTOR_META (effect), name);
  clutter_actor_add_effect (self, effect);
}

/**
 * clutter_actor_remove_effect:
 * @self: a #ClutterActor
 * @effect: a #ClutterEffect
 *
 * Removes @effect from the list of effects applied to @self
 *
 * The reference held by @self on the #ClutterEffect will be released
 *
 * Since: 1.4
 */
void
clutter_actor_remove_effect (ClutterActor  *self,
                             ClutterEffect *effect)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (CLUTTER_IS_EFFECT (effect));

  _clutter_actor_remove_effect_internal (self, effect);

  clutter_actor_queue_redraw (self);

  g_object_notify_by_pspec (G_OBJECT (self), obj_props[PROP_EFFECT]);
}

/**
 * clutter_actor_remove_effect_by_name:
 * @self: a #ClutterActor
 * @name: the name of the effect to remove
 *
 * Removes the #ClutterEffect with the given name from the list
 * of effects applied to @self
 *
 * Since: 1.4
 */
void
clutter_actor_remove_effect_by_name (ClutterActor *self,
                                     const gchar  *name)
{
  ClutterActorPrivate *priv;
  ClutterActorMeta *meta;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (name != NULL);

  priv = self->priv;

  if (priv->effects == NULL)
    return;

  meta = _clutter_meta_group_get_meta (priv->effects, name);
  if (meta == NULL)
    return;

  clutter_actor_remove_effect (self, CLUTTER_EFFECT (meta));
}

/**
 * clutter_actor_get_effects:
 * @self: a #ClutterActor
 *
 * Retrieves the #ClutterEffect<!-- -->s applied on @self, if any
 *
 * Return value: (transfer container) (element-type Clutter.Effect): a list
 *   of #ClutterEffect<!-- -->s, or %NULL. The elements of the returned
 *   list are owned by Clutter and they should not be freed. You should
 *   free the returned list using g_list_free() when done
 *
 * Since: 1.4
 */
GList *
clutter_actor_get_effects (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  priv = self->priv;

  if (priv->effects == NULL)
    return NULL;

  return _clutter_meta_group_get_metas_no_internal (priv->effects);
}

/**
 * clutter_actor_get_effect:
 * @self: a #ClutterActor
 * @name: the name of the effect to retrieve
 *
 * Retrieves the #ClutterEffect with the given name in the list
 * of effects applied to @self
 *
 * Return value: (transfer none): a #ClutterEffect for the given
 *   name, or %NULL. The returned #ClutterEffect is owned by the
 *   actor and it should not be unreferenced directly
 *
 * Since: 1.4
 */
ClutterEffect *
clutter_actor_get_effect (ClutterActor *self,
                          const gchar  *name)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);
  g_return_val_if_fail (name != NULL, NULL);

  if (self->priv->effects == NULL)
    return NULL;

  return CLUTTER_EFFECT (_clutter_meta_group_get_meta (self->priv->effects, name));
}

/**
 * clutter_actor_clear_effects:
 * @self: a #ClutterActor
 *
 * Clears the list of effects applied to @self
 *
 * Since: 1.4
 */
void
clutter_actor_clear_effects (ClutterActor *self)
{
  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  if (self->priv->effects == NULL)
    return;

  _clutter_meta_group_clear_metas_no_internal (self->priv->effects);

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_has_key_focus:
 * @self: a #ClutterActor
 *
 * Checks whether @self is the #ClutterActor that has key focus
 *
 * Return value: %TRUE if the actor has key focus, and %FALSE otherwise
 *
 * Since: 1.4
 */
gboolean
clutter_actor_has_key_focus (ClutterActor *self)
{
  ClutterActor *stage;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);

  stage = _clutter_actor_get_stage_internal (self);
  if (stage == NULL)
    return FALSE;

  return clutter_stage_get_key_focus (CLUTTER_STAGE (stage)) == self;
}

static gboolean
_clutter_actor_get_paint_volume_real (ClutterActor *self,
                                      ClutterPaintVolume *pv)
{
  ClutterActorPrivate *priv = self->priv;

  _clutter_paint_volume_init_static (pv, self);

  if (!CLUTTER_ACTOR_GET_CLASS (self)->get_paint_volume (self, pv))
    {
      clutter_paint_volume_free (pv);
      CLUTTER_NOTE (CLIPPING, "Bail from get_paint_volume (%s): "
                    "Actor failed to report a volume",
                    _clutter_actor_get_debug_name (self));
      return FALSE;
    }

  /* since effects can modify the paint volume, we allow them to actually
   * do this by making get_paint_volume() "context sensitive"
   */
  if (priv->effects != NULL)
    {
      if (priv->current_effect != NULL)
        {
          const GList *effects, *l;

          /* if we are being called from within the paint sequence of
           * an actor, get the paint volume up to the current effect
           */
          effects = _clutter_meta_group_peek_metas (priv->effects);
          for (l = effects;
               l != NULL || (l != NULL && l->data != priv->current_effect);
               l = l->next)
            {
              if (!_clutter_effect_get_paint_volume (l->data, pv))
                {
                  clutter_paint_volume_free (pv);
                  CLUTTER_NOTE (CLIPPING, "Bail from get_paint_volume (%s): "
                                "Effect (%s) failed to report a volume",
                                _clutter_actor_get_debug_name (self),
                                _clutter_actor_meta_get_debug_name (l->data));
                  return FALSE;
                }
            }
        }
      else
        {
          const GList *effects, *l;

          /* otherwise, get the cumulative volume */
          effects = _clutter_meta_group_peek_metas (priv->effects);
          for (l = effects; l != NULL; l = l->next)
            if (!_clutter_effect_get_paint_volume (l->data, pv))
              {
                clutter_paint_volume_free (pv);
                CLUTTER_NOTE (CLIPPING, "Bail from get_paint_volume (%s): "
                              "Effect (%s) failed to report a volume",
                              _clutter_actor_get_debug_name (self),
                              _clutter_actor_meta_get_debug_name (l->data));
                return FALSE;
              }
        }
    }

  return TRUE;
}

/* The public clutter_actor_get_paint_volume API returns a const
 * pointer since we return a pointer directly to the cached
 * PaintVolume associated with the actor and don't want the user to
 * inadvertently modify it, but for internal uses we sometimes need
 * access to the same PaintVolume but need to apply some book-keeping
 * modifications to it so we don't want a const pointer.
 */
static ClutterPaintVolume *
_clutter_actor_get_paint_volume_mutable (ClutterActor *self)
{
  ClutterActorPrivate *priv;

  priv = self->priv;

  if (priv->paint_volume_valid)
    clutter_paint_volume_free (&priv->paint_volume);

  if (_clutter_actor_get_paint_volume_real (self, &priv->paint_volume))
    {
      priv->paint_volume_valid = TRUE;
      return &priv->paint_volume;
    }
  else
    {
      priv->paint_volume_valid = FALSE;
      return NULL;
    }
}

/**
 * clutter_actor_get_paint_volume:
 * @self: a #ClutterActor
 *
 * Retrieves the paint volume of the passed #ClutterActor, or %NULL
 * when a paint volume can't be determined.
 *
 * The paint volume is defined as the 3D space occupied by an actor
 * when being painted.
 *
 * This function will call the <function>get_paint_volume()</function>
 * virtual function of the #ClutterActor class. Sub-classes of #ClutterActor
 * should not usually care about overriding the default implementation,
 * unless they are, for instance: painting outside their allocation, or
 * actors with a depth factor (not in terms of #ClutterActor:depth but real
 * 3D depth).
 *
 * <note>2D actors overriding <function>get_paint_volume()</function>
 * ensure their volume has a depth of 0. (This will be true so long as
 * you don't call clutter_paint_volume_set_depth().)</note>
 *
 * Return value: (transfer none): a pointer to a #ClutterPaintVolume,
 *   or %NULL if no volume could be determined. The returned pointer
 *   is not guaranteed to be valid across multiple frames; if you want
 *   to keep it, you will need to copy it using clutter_paint_volume_copy().
 *
 * Since: 1.6
 */
const ClutterPaintVolume *
clutter_actor_get_paint_volume (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return _clutter_actor_get_paint_volume_mutable (self);
}

/**
 * clutter_actor_get_transformed_paint_volume:
 * @self: a #ClutterActor
 * @relative_to_ancestor: A #ClutterActor that is an ancestor of @self
 *    (or %NULL for the stage)
 *
 * Retrieves the 3D paint volume of an actor like
 * clutter_actor_get_paint_volume() does (Please refer to the
 * documentation of clutter_actor_get_paint_volume() for more
 * details.) and it additionally transforms the paint volume into the
 * coordinate space of @relative_to_ancestor. (Or the stage if %NULL
 * is passed for @relative_to_ancestor)
 *
 * This can be used by containers that base their paint volume on
 * the volume of their children. Such containers can query the
 * transformed paint volume of all of its children and union them
 * together using clutter_paint_volume_union().
 *
 * Return value: (transfer none): a pointer to a #ClutterPaintVolume,
 *   or %NULL if no volume could be determined. The returned pointer is
 *   not guaranteed to be valid across multiple frames; if you wish to
 *   keep it, you will have to copy it using clutter_paint_volume_copy().
 *
 * Since: 1.6
 */
const ClutterPaintVolume *
clutter_actor_get_transformed_paint_volume (ClutterActor *self,
                                            ClutterActor *relative_to_ancestor)
{
  const ClutterPaintVolume *volume;
  ClutterActor *stage;
  ClutterPaintVolume *transformed_volume;

  stage = _clutter_actor_get_stage_internal (self);
  if (G_UNLIKELY (stage == NULL))
    return NULL;

  if (relative_to_ancestor == NULL)
    relative_to_ancestor = stage;

  volume = clutter_actor_get_paint_volume (self);
  if (volume == NULL)
    return NULL;

  transformed_volume =
    _clutter_stage_paint_volume_stack_allocate (CLUTTER_STAGE (stage));

  _clutter_paint_volume_copy_static (volume, transformed_volume);

  _clutter_paint_volume_transform_relative (transformed_volume,
                                            relative_to_ancestor);

  return transformed_volume;
}

/**
 * clutter_actor_get_paint_box:
 * @self: a #ClutterActor
 * @box: (out): return location for a #ClutterActorBox
 *
 * Retrieves the paint volume of the passed #ClutterActor, and
 * transforms it into a 2D bounding box in stage coordinates.
 *
 * This function is useful to determine the on screen area occupied by
 * the actor. The box is only an approximation and may often be
 * considerably larger due to the optimizations used to calculate the
 * box. The box is never smaller though, so it can reliably be used
 * for culling.
 *
 * There are times when a 2D paint box can't be determined, e.g.
 * because the actor isn't yet parented under a stage or because
 * the actor is unable to determine a paint volume.
 *
 * Return value: %TRUE if a 2D paint box could be determined, else
 * %FALSE.
 *
 * Since: 1.6
 */
gboolean
clutter_actor_get_paint_box (ClutterActor    *self,
                             ClutterActorBox *box)
{
  ClutterActor *stage;
  ClutterPaintVolume *pv;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), FALSE);
  g_return_val_if_fail (box != NULL, FALSE);

  stage = _clutter_actor_get_stage_internal (self);
  if (G_UNLIKELY (!stage))
    return FALSE;

  pv = _clutter_actor_get_paint_volume_mutable (self);
  if (G_UNLIKELY (!pv))
    return FALSE;

  _clutter_paint_volume_get_stage_paint_box (pv, CLUTTER_STAGE (stage), box);

  return TRUE;
}

/**
 * clutter_actor_has_overlaps:
 * @self: A #ClutterActor
 *
 * Asks the actor's implementation whether it may contain overlapping
 * primitives.
 *
 * For example; Clutter may use this to determine whether the painting
 * should be redirected to an offscreen buffer to correctly implement
 * the opacity property.
 *
 * Custom actors can override the default response by implementing the
 * #ClutterActor <function>has_overlaps</function> virtual function. See
 * clutter_actor_set_offscreen_redirect() for more information.
 *
 * Return value: %TRUE if the actor may have overlapping primitives, and
 *   %FALSE otherwise
 *
 * Since: 1.8
 */
gboolean
clutter_actor_has_overlaps (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), TRUE);

  return CLUTTER_ACTOR_GET_CLASS (self)->has_overlaps (self);
}

/**
 * clutter_actor_has_effects:
 * @self: A #ClutterActor
 *
 * Returns whether the actor has any effects applied.
 *
 * Return value: %TRUE if the actor has any effects,
 *   %FALSE otherwise
 *
 * Since: 1.10
 */
gboolean
clutter_actor_has_effects (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), TRUE);

  if (self->priv->effects == NULL)
    return FALSE;

  return _clutter_meta_group_has_metas_no_internal (self->priv->effects);
}

/**
 * clutter_actor_get_n_children:
 * @self: a #ClutterActor
 *
 * Retrieves the number of children of @self.
 *
 * Return value: the number of children of an actor
 *
 * Since: 1.10
 */
gint
clutter_actor_get_n_children (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  return self->priv->n_children;
}

/**
 * clutter_actor_get_child_at_index:
 * @self: a #ClutterActor
 * @index_: the position in the list of children
 *
 * Retrieves the actor at the given @index_ inside the list of
 * children of @self.
 *
 * Return value: (transfer none): a pointer to a #ClutterActor, or %NULL
 *
 * Since: 1.10
 */
ClutterActor *
clutter_actor_get_child_at_index (ClutterActor *self,
                                  gint          index_)
{
  ClutterActor *iter;
  int i;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);
  g_return_val_if_fail (index_ <= self->priv->n_children, NULL);

  for (iter = self->priv->first_child, i = 0;
       iter != NULL && i < index_;
       iter = iter->priv->next_sibling, i += 1)
    ;

  return iter;
}

/*< private >
 * _clutter_actor_foreach_child:
 * @actor: The actor whos children you want to iterate
 * @callback: The function to call for each child
 * @user_data: Private data to pass to @callback
 *
 * Calls a given @callback once for each child of the specified @actor and
 * passing the @user_data pointer each time.
 *
 * Return value: returns %TRUE if all children were iterated, else
 *    %FALSE if a callback broke out of iteration early.
 */
gboolean
_clutter_actor_foreach_child (ClutterActor           *self,
                              ClutterForeachCallback  callback,
                              gpointer                user_data)
{
  ClutterActor *iter;
  gboolean cont;

  if (self->priv->first_child == NULL)
    return TRUE;

  cont = TRUE;
  iter = self->priv->first_child;

  /* we use this form so that it's safe to change the children
   * list while iterating it
   */
  while (cont && iter != NULL)
    {
      ClutterActor *next = iter->priv->next_sibling;

      cont = callback (iter, user_data);

      iter = next;
    }

  return cont;
}

#if 0
/* For debugging purposes this gives us a simple way to print out
 * the scenegraph e.g in gdb using:
 * [|
 *   _clutter_actor_traverse (stage,
 *                            0,
 *                            clutter_debug_print_actor_cb,
 *                            NULL,
 *                            NULL);
 * |]
 */
static ClutterActorTraverseVisitFlags
clutter_debug_print_actor_cb (ClutterActor *actor,
                              int depth,
                              void *user_data)
{
  g_print ("%*s%s:%p\n",
           depth * 2, "",
           _clutter_actor_get_debug_name (actor),
           actor);

  return CLUTTER_ACTOR_TRAVERSE_VISIT_CONTINUE;
}
#endif

static void
_clutter_actor_traverse_breadth (ClutterActor           *actor,
                                 ClutterTraverseCallback callback,
                                 gpointer                user_data)
{
  GQueue *queue = g_queue_new ();
  ClutterActor dummy;
  int current_depth = 0;

  g_queue_push_tail (queue, actor);
  g_queue_push_tail (queue, &dummy); /* use to delimit depth changes */

  while ((actor = g_queue_pop_head (queue)))
    {
      ClutterActorTraverseVisitFlags flags;

      if (actor == &dummy)
        {
          current_depth++;
          g_queue_push_tail (queue, &dummy);
          continue;
        }

      flags = callback (actor, current_depth, user_data);
      if (flags & CLUTTER_ACTOR_TRAVERSE_VISIT_BREAK)
        break;
      else if (!(flags & CLUTTER_ACTOR_TRAVERSE_VISIT_SKIP_CHILDREN))
        {
          ClutterActor *iter;

          for (iter = actor->priv->first_child;
               iter != NULL;
               iter = iter->priv->next_sibling)
            {
              g_queue_push_tail (queue, iter);
            }
        }
    }

  g_queue_free (queue);
}

static ClutterActorTraverseVisitFlags
_clutter_actor_traverse_depth (ClutterActor           *actor,
                               ClutterTraverseCallback before_children_callback,
                               ClutterTraverseCallback after_children_callback,
                               int                     current_depth,
                               gpointer                user_data)
{
  ClutterActorTraverseVisitFlags flags;

  flags = before_children_callback (actor, current_depth, user_data);
  if (flags & CLUTTER_ACTOR_TRAVERSE_VISIT_BREAK)
    return CLUTTER_ACTOR_TRAVERSE_VISIT_BREAK;

  if (!(flags & CLUTTER_ACTOR_TRAVERSE_VISIT_SKIP_CHILDREN))
    {
      ClutterActor *iter;

      for (iter = actor->priv->first_child;
           iter != NULL;
           iter = iter->priv->next_sibling)
        {
          flags = _clutter_actor_traverse_depth (iter,
                                                 before_children_callback,
                                                 after_children_callback,
                                                 current_depth + 1,
                                                 user_data);

          if (flags & CLUTTER_ACTOR_TRAVERSE_VISIT_BREAK)
            return CLUTTER_ACTOR_TRAVERSE_VISIT_BREAK;
        }
    }

  if (after_children_callback)
    return after_children_callback (actor, current_depth, user_data);
  else
    return CLUTTER_ACTOR_TRAVERSE_VISIT_CONTINUE;
}

/* _clutter_actor_traverse:
 * @actor: The actor to start traversing the graph from
 * @flags: These flags may affect how the traversal is done
 * @before_children_callback: A function to call before visiting the
 *   children of the current actor.
 * @after_children_callback: A function to call after visiting the
 *   children of the current actor. (Ignored if
 *   %CLUTTER_ACTOR_TRAVERSE_BREADTH_FIRST is passed to @flags.)
 * @user_data: The private data to pass to the callbacks
 *
 * Traverses the scenegraph starting at the specified @actor and
 * descending through all its children and its children's children.
 * For each actor traversed @before_children_callback and
 * @after_children_callback are called with the specified
 * @user_data, before and after visiting that actor's children.
 *
 * The callbacks can return flags that affect the ongoing traversal
 * such as by skipping over an actors children or bailing out of
 * any further traversing.
 */
void
_clutter_actor_traverse (ClutterActor              *actor,
                         ClutterActorTraverseFlags  flags,
                         ClutterTraverseCallback    before_children_callback,
                         ClutterTraverseCallback    after_children_callback,
                         gpointer                   user_data)
{
  if (flags & CLUTTER_ACTOR_TRAVERSE_BREADTH_FIRST)
    _clutter_actor_traverse_breadth (actor,
                                     before_children_callback,
                                     user_data);
  else /* DEPTH_FIRST */
    _clutter_actor_traverse_depth (actor,
                                   before_children_callback,
                                   after_children_callback,
                                   0, /* start depth */
                                   user_data);
}

static inline void
clutter_actor_set_background_color_internal (ClutterActor *self,
                                             const ClutterColor *color)
{
  ClutterActorPrivate *priv = self->priv;
  ClutterTransformInfo *info = _clutter_actor_get_transform_info (self);

  if (priv->bg_color_set && clutter_color_equal (color, &info->bg_color))
    return;

  info->bg_color = *color;
  priv->bg_color_set = TRUE;

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_set_background_color:
 * @self: a #ClutterActor
 * @color: (allow-none): a #ClutterColor, or %NULL to unset a previously
 *  set color
 *
 * Sets the background color of a #ClutterActor.
 *
 * The background color will be used to cover the whole allocation of the
 * actor. The default background color of an actor is transparent.
 *
 * To check whether an actor has a background color, you can use the
 * #ClutterActor:background-color-set actor property.
 *
 * The #ClutterActor:background-color property is animatable.
 *
 * Since: 1.10
 */
void
clutter_actor_set_background_color (ClutterActor       *self,
                                    const ClutterColor *color)
{
  ClutterActorPrivate *priv;
  GObject *obj;
  GParamSpec *bg_color_pspec;
  ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  obj = G_OBJECT (self);
  priv = self->priv;
  info = _clutter_actor_get_transform_info (self);

  if (color == NULL)
    {
      priv->bg_color_set = FALSE;
      g_object_notify_by_pspec (obj, obj_props[PROP_BACKGROUND_COLOR_SET]);
      clutter_actor_queue_redraw (self);
      return;
    }

  bg_color_pspec = obj_anim_props[PROP_BACKGROUND_COLOR];
  if (_clutter_actor_get_transition (self, bg_color_pspec) == NULL)
    {
      _clutter_actor_create_transition (self, bg_color_pspec,
                                        &info->bg_color,
                                        color);
    }
  else
    _clutter_actor_update_transition (self, bg_color_pspec, color);

  g_object_notify_by_pspec (obj, obj_props[PROP_BACKGROUND_COLOR_SET]);

  clutter_actor_queue_redraw (self);
}

/**
 * clutter_actor_get_background_color:
 * @self: a #ClutterActor
 * @color: (out caller-allocates): return location for a #ClutterColor
 *
 * Retrieves the color set using clutter_actor_set_background_color().
 *
 * Since: 1.10
 */
void
clutter_actor_get_background_color (ClutterActor *self,
                                    ClutterColor *color)
{
  const ClutterTransformInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (color != NULL);

  info = _clutter_actor_get_transform_info_or_defaults (self);

  *color = info->bg_color;
}

/**
 * clutter_actor_get_previous_sibling:
 * @self: a #ClutterActor
 *
 * Retrieves the sibling of @self that comes before it in the list
 * of children of @self's parent.
 *
 * The returned pointer is only valid until the scene graph changes; it
 * is not safe to modify the list of children of @self while iterating
 * it.
 *
 * Return value: (transfer none): a pointer to a #ClutterActor, or %NULL
 *
 * Since: 1.10
 */
ClutterActor *
clutter_actor_get_previous_sibling (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return self->priv->prev_sibling;
}

/**
 * clutter_actor_get_next_sibling:
 * @self: a #ClutterActor
 *
 * Retrieves the sibling of @self that comes after it in the list
 * of children of @self's parent.
 *
 * The returned pointer is only valid until the scene graph changes; it
 * is not safe to modify the list of children of @self while iterating
 * it.
 *
 * Return value: (transfer none): a pointer to a #ClutterActor, or %NULL
 *
 * Since: 1.10
 */
ClutterActor *
clutter_actor_get_next_sibling (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return self->priv->next_sibling;
}

/**
 * clutter_actor_get_first_child:
 * @self: a #ClutterActor
 *
 * Retrieves the first child of @self.
 *
 * The returned pointer is only valid until the scene graph changes; it
 * is not safe to modify the list of children of @self while iterating
 * it.
 *
 * Return value: (transfer none): a pointer to a #ClutterActor, or %NULL
 *
 * Since: 1.10
 */
ClutterActor *
clutter_actor_get_first_child (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return self->priv->first_child;
}

/**
 * clutter_actor_get_last_child:
 * @self: a #ClutterActor
 *
 * Retrieves the last child of @self.
 *
 * The returned pointer is only valid until the scene graph changes; it
 * is not safe to modify the list of children of @self while iterating
 * it.
 *
 * Return value: (transfer none): a pointer to a #ClutterActor, or %NULL
 *
 * Since: 1.10
 */
ClutterActor *
clutter_actor_get_last_child (ClutterActor *self)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);

  return self->priv->last_child;
}

/* easy way to have properly named fields instead of the dummy ones
 * we use in the public structure
 */
typedef struct _RealActorIter
{
  ClutterActor *root;           /* dummy1 */
  ClutterActor *current;        /* dummy2 */
  gpointer padding_1;           /* dummy3 */
  gint age;                     /* dummy4 */
  gpointer padding_2;           /* dummy5 */
} RealActorIter;

/**
 * clutter_actor_iter_init:
 * @iter: a #ClutterActorIter
 * @root: a #ClutterActor
 *
 * Initializes a #ClutterActorIter, which can then be used to iterate
 * efficiently over a section of the scene graph, and associates it
 * with @root.
 *
 * Modifying the scene graph section that contains @root will invalidate
 * the iterator.
 *
 * |[
 *   ClutterActorIter iter;
 *   ClutterActor *child;
 *
 *   clutter_actor_iter_init (&iter, container);
 *   while (clutter_actor_iter_next (&iter, &child))
 *     {
 *       /&ast; do something with child &ast;/
 *     }
 * ]|
 *
 * Since: 1.10
 */
void
clutter_actor_iter_init (ClutterActorIter *iter,
                         ClutterActor     *root)
{
  RealActorIter *ri = (RealActorIter *) iter;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (CLUTTER_IS_ACTOR (root));

  ri->root = root;
  ri->current = NULL;
  ri->age = root->priv->age;
}

/**
 * clutter_actor_iter_next:
 * @iter: a #ClutterActorIter
 * @child: (out): return location for a #ClutterActor
 *
 * Advances the @iter and retrieves the next child of the root #ClutterActor
 * that was used to initialize the #ClutterActorIterator.
 *
 * If the iterator can advance, this function returns %TRUE and sets the
 * @child argument.
 *
 * If the iterator cannot advance, this function returns %FALSE, and
 * the contents of @child are undefined.
 *
 * Return value: %TRUE if the iterator could advance, and %FALSE otherwise.
 *
 * Since: 1.10
 */
gboolean
clutter_actor_iter_next (ClutterActorIter  *iter,
                         ClutterActor     **child)
{
  RealActorIter *ri = (RealActorIter *) iter;

  g_return_val_if_fail (iter != NULL, FALSE);
  g_return_val_if_fail (ri->root != NULL, FALSE);
#ifndef G_DISABLE_ASSERT
  g_return_val_if_fail (ri->age == ri->root->priv->age, FALSE);
#endif

  if (ri->current == NULL)
    ri->current = ri->root->priv->first_child;
  else
    ri->current = ri->current->priv->next_sibling;

  if (child != NULL)
    *child = ri->current;

  return ri->current != NULL;
}

/**
 * clutter_actor_iter_prev:
 * @iter: a #ClutterActorIter
 * @child: (out): return location for a #ClutterActor
 *
 * Advances the @iter and retrieves the previous child of the root
 * #ClutterActor that was used to initialize the #ClutterActorIterator.
 *
 * If the iterator can advance, this function returns %TRUE and sets the
 * @child argument.
 *
 * If the iterator cannot advance, this function returns %FALSE, and
 * the contents of @child are undefined.
 *
 * Return value: %TRUE if the iterator could advance, and %FALSE otherwise.
 *
 * Since: 1.10
 */
gboolean
clutter_actor_iter_prev (ClutterActorIter  *iter,
                         ClutterActor     **child)
{
  RealActorIter *ri = (RealActorIter *) iter;

  g_return_val_if_fail (iter != NULL, FALSE);
  g_return_val_if_fail (ri->root != NULL, FALSE);
#ifndef G_DISABLE_ASSERT
  g_return_val_if_fail (ri->age == ri->root->priv->age, FALSE);
#endif

  if (ri->current == NULL)
    ri->current = ri->root->priv->last_child;
  else
    ri->current = ri->current->priv->prev_sibling;

  if (child != NULL)
    *child = ri->current;

  return ri->current != NULL;
}

/**
 * clutter_actor_iter_remove:
 * @iter: a #ClutterActorIter
 *
 * Safely removes the #ClutterActor currently pointer to by the iterator
 * from its parent.
 *
 * This function can only be called after clutter_actor_iter_next() or
 * clutter_actor_iter_prev() returned %TRUE, and cannot be called more
 * than once for the same actor.
 *
 * This function will call clutter_actor_remove_child() internally.
 *
 * Since: 1.10
 */
void
clutter_actor_iter_remove (ClutterActorIter *iter)
{
  RealActorIter *ri = (RealActorIter *) iter;
  ClutterActor *cur;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (ri->root != NULL);
#ifndef G_DISABLE_ASSERT
  g_return_if_fail (ri->age == ri->root->priv->age);
#endif
  g_return_if_fail (ri->current != NULL);

  cur = ri->current;

  if (cur != NULL)
    {
      ri->current = cur->priv->prev_sibling;

      clutter_actor_remove_child_internal (ri->root, cur,
                                           REMOVE_CHILD_DEFAULT_FLAGS);

      ri->age += 1;
    }
}

/**
 * clutter_actor_iter_destroy:
 * @iter: a #ClutterActorIter
 *
 * Safely destroys the #ClutterActor currently pointer to by the iterator
 * from its parent.
 *
 * This function can only be called after clutter_actor_iter_next() or
 * clutter_actor_iter_prev() returned %TRUE, and cannot be called more
 * than once for the same actor.
 *
 * This function will call clutter_actor_destroy() internally.
 *
 * Since: 1.10
 */
void
clutter_actor_iter_destroy (ClutterActorIter *iter)
{
  RealActorIter *ri = (RealActorIter *) iter;
  ClutterActor *cur;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (ri->root != NULL);
#ifndef G_DISABLE_ASSERT
  g_return_if_fail (ri->age == ri->root->priv->age);
#endif
  g_return_if_fail (ri->current != NULL);

  cur = ri->current;

  if (cur != NULL)
    {
      ri->current = cur->priv->prev_sibling;

      clutter_actor_destroy (cur);

      ri->age += 1;
    }
}

static gpointer
clutter_actor_iter_copy (gpointer data)
{
  return g_slice_dup (ClutterActorIter, data);
}

static void
clutter_actor_iter_free (gpointer data)
{
  if (G_LIKELY (data))
    g_slice_free (ClutterActorIter, data);
}

GType
clutter_actor_iter_get_type (void)
{
  static GType our_type = 0;

  if (G_UNLIKELY (our_type == 0))
    our_type = g_boxed_type_register_static (g_intern_static_string ("ClutterActorIter"),
                                             clutter_actor_iter_copy,
                                             clutter_actor_iter_free);

  return our_type;
}


static const ClutterAnimationInfo default_animation_info = {
  NULL,         /* transitions */
  NULL,         /* states */
  NULL,         /* cur_state */
};

static void
clutter_animation_info_free (gpointer data)
{
  if (data != NULL)
    {
      ClutterAnimationInfo *info = data;

      if (info->transitions != NULL)
        g_hash_table_unref (info->transitions);

      if (info->states != NULL)
        g_array_unref (info->states);

      g_slice_free (ClutterAnimationInfo, info);
    }
}

const ClutterAnimationInfo *
_clutter_actor_get_animation_info_or_defaults (ClutterActor *self)
{
  const ClutterAnimationInfo *res;
  GObject *obj = G_OBJECT (self);

  res = g_object_get_qdata (obj, quark_actor_animation_info);
  if (res != NULL)
    return res;

  return &default_animation_info;
}

ClutterAnimationInfo *
_clutter_actor_get_animation_info (ClutterActor *self)
{
  GObject *obj = G_OBJECT (self);
  ClutterAnimationInfo *res;

  res = g_object_get_qdata (obj, quark_actor_animation_info);
  if (res == NULL)
    {
      res = g_slice_new (ClutterAnimationInfo);

      *res = default_animation_info;

      g_object_set_qdata_full (obj, quark_actor_animation_info,
                               res,
                               clutter_animation_info_free);
    }

  return res;
}

ClutterTransition *
_clutter_actor_get_transition (ClutterActor *actor,
                               GParamSpec   *pspec)
{
  const ClutterAnimationInfo *info;

  info = _clutter_actor_get_animation_info_or_defaults (actor);

  if (info->transitions == NULL)
    return NULL;

  return g_hash_table_lookup (info->transitions, pspec->name);
}

typedef struct _TransitionClosure
{
  ClutterActor *actor;
  ClutterTransition *transition;
  gchar *name;
  gulong completed_id;
} TransitionClosure;

static void
transition_closure_free (gpointer data)
{
  if (G_LIKELY (data != NULL))
    {
      TransitionClosure *clos = data;
      ClutterTimeline *timeline;

      timeline = CLUTTER_TIMELINE (clos->transition);

      if (clutter_timeline_is_playing (timeline))
        clutter_timeline_stop (timeline);

      g_signal_handler_disconnect (clos->transition, clos->completed_id);

      g_object_unref (clos->transition);
      g_free (clos->name);

      g_slice_free (TransitionClosure, clos);
    }
}

static void
on_transition_completed (ClutterTransition *transition,
                         TransitionClosure *clos)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (transition);
  ClutterActor *actor = clos->actor;
  ClutterAnimationInfo *info;
  gint n_repeats, cur_repeat;

  info = _clutter_actor_get_animation_info (actor);

  /* ensure that we remove the transition only at the end
   * of its run; we emit ::completed for every repeat
   */
  n_repeats = clutter_timeline_get_repeat_count (timeline);
  cur_repeat = clutter_timeline_get_current_repeat (timeline);

  if (cur_repeat == n_repeats)
    {
      if (clutter_transition_get_remove_on_complete (transition))
        {
          /* we take a reference here because removing the closure
           * will release the reference on the transition, and we
           * want the transition to survive the signal emission;
           * the master clock will release the last reference at
           * the end of the frame processing.
           */
          g_object_ref (transition);
          g_hash_table_remove (info->transitions, clos->name);
        }
    }

  /* if it's the last transition then we clean up */
  if (g_hash_table_size (info->transitions) == 0)
    {
      g_hash_table_unref (info->transitions);
      info->transitions = NULL;

      CLUTTER_NOTE (ANIMATION, "Transitions for '%s' completed",
                    _clutter_actor_get_debug_name (actor));

      g_signal_emit (actor, actor_signals[TRANSITIONS_COMPLETED], 0);
    }
}

void
_clutter_actor_update_transition (ClutterActor *actor,
                                  GParamSpec   *pspec,
                                  ...)
{
  TransitionClosure *clos;
  ClutterTimeline *timeline;
  ClutterInterval *interval;
  const ClutterAnimationInfo *info;
  va_list var_args;
  GType ptype;
  GValue initial = G_VALUE_INIT;
  GValue final = G_VALUE_INIT;
  char *error = NULL;

  info = _clutter_actor_get_animation_info_or_defaults (actor);

  if (info->transitions == NULL)
    return;

  clos = g_hash_table_lookup (info->transitions, pspec->name);
  if (clos == NULL)
    return;

  timeline = CLUTTER_TIMELINE (clos->transition);

  va_start (var_args, pspec);

  ptype = G_PARAM_SPEC_VALUE_TYPE (pspec);

  g_value_init (&initial, ptype);
  clutter_animatable_get_initial_state (CLUTTER_ANIMATABLE (actor),
                                        pspec->name,
                                        &initial);

  G_VALUE_COLLECT_INIT (&final, ptype, var_args, 0, &error);
  if (error != NULL)
    {
      g_critical ("%s: %s", G_STRLOC, error);
      g_free (error);
      goto out;
    }

  interval = clutter_transition_get_interval (clos->transition);
  clutter_interval_set_initial_value (interval, &initial);
  clutter_interval_set_final_value (interval, &final);

  /* if we're updating with an easing duration of zero milliseconds,
   * we just jump the timeline to the end and let it run its course
   */
  if (info->cur_state != NULL &&
      info->cur_state->easing_duration != 0)
    {
      guint cur_duration = clutter_timeline_get_duration (timeline);
      ClutterAnimationMode cur_mode =
        clutter_timeline_get_progress_mode (timeline);

      if (cur_duration != info->cur_state->easing_duration)
        clutter_timeline_set_duration (timeline, info->cur_state->easing_duration);

      if (cur_mode != info->cur_state->easing_mode)
        clutter_timeline_set_progress_mode (timeline, info->cur_state->easing_mode);

      clutter_timeline_rewind (timeline);
    }
  else
    {
      guint duration = clutter_timeline_get_duration (timeline);

      clutter_timeline_advance (timeline, duration);
    }

out:
  g_value_unset (&initial);
  g_value_unset (&final);

  va_end (var_args);
}

/*< private >*
 * _clutter_actor_create_transition:
 * @actor: a #ClutterActor
 * @pspec: the property used for the transition
 * @...: initial and final state
 *
 * Creates a #ClutterTransition for the property represented by @pspec.
 *
 * Return value: a #ClutterTransition
 */
ClutterTransition *
_clutter_actor_create_transition (ClutterActor *actor,
                                  GParamSpec   *pspec,
                                  ...)
{
  ClutterAnimationInfo *info;
  ClutterTransition *res = NULL;
  gboolean call_restore = FALSE;
  TransitionClosure *clos;
  va_list var_args;

  info = _clutter_actor_get_animation_info (actor);

  /* XXX - this will go away in 2.0
   *
   * if no state has been pushed, we assume that the easing state is
   * in "compatibility mode": all transitions have a duration of 0
   * msecs, which means that they happen immediately. in Clutter 2.0
   * this will turn into a g_assert(info->states != NULL), as every
   * actor will start with a predefined easing state
   */
  if (info->states == NULL)
    {
      clutter_actor_save_easing_state (actor);
      clutter_actor_set_easing_duration (actor, 0);
      call_restore = TRUE;
    }

  if (info->transitions == NULL)
    info->transitions = g_hash_table_new_full (g_str_hash, g_str_equal,
                                               NULL,
                                               transition_closure_free);

  va_start (var_args, pspec);

  clos = g_hash_table_lookup (info->transitions, pspec->name);
  if (clos == NULL)
    {
      ClutterInterval *interval;
      GValue initial = G_VALUE_INIT;
      GValue final = G_VALUE_INIT;
      GType ptype;
      char *error;

      ptype = G_PARAM_SPEC_VALUE_TYPE (pspec);

      G_VALUE_COLLECT_INIT (&initial, ptype,
                            var_args, 0,
                            &error);
      if (error != NULL)
        {
          g_critical ("%s: %s", G_STRLOC, error);
          g_free (error);
          goto out;
        }

      G_VALUE_COLLECT_INIT (&final, ptype,
                            var_args, 0,
                            &error);

      if (error != NULL)
        {
          g_critical ("%s: %s", G_STRLOC, error);
          g_value_unset (&initial);
          g_free (error);
          goto out;
        }

      /* if the current easing state has a duration of 0, then we don't
       * bother to create the transition, and we just set the final value
       * directly on the actor; we don't go through the Animatable
       * interface because we know we got here through an animatable
       * property.
       */
      if (info->cur_state->easing_duration == 0)
        {
          clutter_actor_set_animatable_property (actor,
                                                 pspec->param_id,
                                                 &final,
                                                 pspec);
          g_value_unset (&initial);
          g_value_unset (&final);

          goto out;
        }

      interval = clutter_interval_new_with_values (ptype, &initial, &final);

      g_value_unset (&initial);
      g_value_unset (&final);

      res = clutter_property_transition_new (pspec->name);

      clutter_transition_set_interval (res, interval);
      clutter_transition_set_remove_on_complete (res, TRUE);

      /* this will start the transition as well */
      clutter_actor_add_transition (actor, pspec->name, res);

      /* the actor now owns the transition */
      g_object_unref (res);
    }
  else
    res = clos->transition;

out:
  if (call_restore)
    clutter_actor_restore_easing_state (actor);

  va_end (var_args);

  return res;
}

/**
 * clutter_actor_add_transition:
 * @self: a #ClutterActor
 * @name: the name of the transition to add
 * @transition: the #ClutterTransition to add
 *
 * Adds a @transition to the #ClutterActor's list of animations.
 *
 * The @name string is a per-actor unique identifier of the @transition: only
 * one #ClutterTransition can be associated to the specified @name.
 *
 * The @transition will be given the easing duration, mode, and delay
 * associated to the actor's current easing state; it is possible to modify
 * these values after calling clutter_actor_add_transition().
 *
 * The @transition will be started once added.
 *
 * This function will take a reference on the @transition.
 *
 * This function is usually called implicitly when modifying an animatable
 * property.
 *
 * Since: 1.10
 */
void
clutter_actor_add_transition (ClutterActor      *self,
                              const char        *name,
                              ClutterTransition *transition)
{
  ClutterTimeline *timeline;
  TransitionClosure *clos;
  ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (name != NULL);
  g_return_if_fail (CLUTTER_IS_TRANSITION (transition));

  info = _clutter_actor_get_animation_info (self);

  if (info->cur_state == NULL)
    {
      g_warning ("No easing state is defined for the actor '%s'; you "
                 "must call clutter_actor_save_easing_state() before "
                 "calling clutter_actor_add_transition().",
                 _clutter_actor_get_debug_name (self));
      return;
    }

  if (info->transitions == NULL)
    info->transitions = g_hash_table_new_full (g_str_hash, g_str_equal,
                                               NULL,
                                               transition_closure_free);

  if (g_hash_table_lookup (info->transitions, name) != NULL)
    {
      g_warning ("A transition with name '%s' already exists for "
                 "the actor '%s'",
                 name,
                 _clutter_actor_get_debug_name (self));
      return;
    }

  clutter_transition_set_animatable (transition, CLUTTER_ANIMATABLE (self));

  timeline = CLUTTER_TIMELINE (transition);

  clutter_timeline_set_delay (timeline, info->cur_state->easing_delay);
  clutter_timeline_set_duration (timeline, info->cur_state->easing_duration);
  clutter_timeline_set_progress_mode (timeline, info->cur_state->easing_mode);

  clos = g_slice_new (TransitionClosure);
  clos->actor = self;
  clos->transition = g_object_ref (transition);
  clos->name = g_strdup (name);
  clos->completed_id = g_signal_connect (timeline, "completed",
                                         G_CALLBACK (on_transition_completed),
                                         clos);

  CLUTTER_NOTE (ANIMATION,
                "Adding transition '%s' [%p] to actor '%s'",
                clos->name,
                clos->transition,
                _clutter_actor_get_debug_name (self));

  g_hash_table_insert (info->transitions, clos->name, clos);
  clutter_timeline_start (timeline);
}

/**
 * clutter_actor_remove_transition:
 * @self: a #ClutterActor
 * @name: the name of the transition to remove
 *
 * Removes the transition stored inside a #ClutterActor using @name
 * identifier.
 *
 * If the transition is currently in progress, it will be stopped.
 *
 * This function releases the reference acquired when the transition
 * was added to the #ClutterActor.
 *
 * Since: 1.10
 */
void
clutter_actor_remove_transition (ClutterActor *self,
                                 const char   *name)
{
  const ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (name != NULL);

  info = _clutter_actor_get_animation_info_or_defaults (self);

  if (info->transitions == NULL)
    return;

  g_hash_table_remove (info->transitions, name);
}

/**
 * clutter_actor_remove_all_transitions:
 * @self: a #ClutterActor
 *
 * Removes all transitions associated to @self.
 *
 * Since: 1.10
 */
void
clutter_actor_remove_all_transitions (ClutterActor *self)
{
  const ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_animation_info_or_defaults (self);
  if (info->transitions == NULL)
    return;

  g_hash_table_remove_all (info->transitions);
}

/**
 * clutter_actor_set_easing_duration:
 * @self: a #ClutterActor
 * @msecs: the duration of the easing, or %NULL
 *
 * Sets the duration of the tweening for animatable properties
 * of @self for the current easing state.
 *
 * Since: 1.10
 */
void
clutter_actor_set_easing_duration (ClutterActor *self,
                                   guint         msecs)
{
  ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_animation_info (self);

  if (info->cur_state == NULL)
    {
      g_warning ("You must call clutter_actor_save_easing_state() prior "
                 "to calling clutter_actor_set_easing_duration().");
      return;
    }

  if (info->cur_state->easing_duration != msecs)
    info->cur_state->easing_duration = msecs;
}

/**
 * clutter_actor_get_easing_duration:
 * @self: a #ClutterActor
 *
 * Retrieves the duration of the tweening for animatable
 * properties of @self for the current easing state.
 *
 * Return value: the duration of the tweening, in milliseconds
 *
 * Since: 1.10
 */
guint
clutter_actor_get_easing_duration (ClutterActor *self)
{
  const ClutterAnimationInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_animation_info_or_defaults (self);

  if (info->cur_state != NULL)
    return info->cur_state->easing_duration;

  return 0;
}

/**
 * clutter_actor_set_easing_mode:
 * @self: a #ClutterActor
 * @mode: an easing mode, excluding %CLUTTER_CUSTOM_MODE
 *
 * Sets the easing mode for the tweening of animatable properties
 * of @self.
 *
 * Since: 1.10
 */
void
clutter_actor_set_easing_mode (ClutterActor         *self,
                               ClutterAnimationMode  mode)
{
  ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));
  g_return_if_fail (mode != CLUTTER_CUSTOM_MODE);
  g_return_if_fail (mode < CLUTTER_ANIMATION_LAST);

  info = _clutter_actor_get_animation_info (self);

  if (info->cur_state == NULL)
    {
      g_warning ("You must call clutter_actor_save_easing_state() prior "
                 "to calling clutter_actor_set_easing_mode().");
      return;
    }

  if (info->cur_state->easing_mode != mode)
    info->cur_state->easing_mode = mode;
}

/**
 * clutter_actor_get_easing_mode:
 * @self: a #ClutterActor
 *
 * Retrieves the easing mode for the tweening of animatable properties
 * of @self for the current easing state.
 *
 * Return value: an easing mode
 *
 * Since: 1.10
 */
ClutterAnimationMode
clutter_actor_get_easing_mode (ClutterActor *self)
{
  const ClutterAnimationInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), CLUTTER_EASE_OUT_CUBIC);

  info = _clutter_actor_get_animation_info_or_defaults (self);

  if (info->cur_state != NULL)
    return info->cur_state->easing_mode;

  return CLUTTER_EASE_OUT_CUBIC;
}

/**
 * clutter_actor_set_easing_delay:
 * @self: a #ClutterActor
 * @msecs: the delay before the start of the tweening, in milliseconds
 *
 * Sets the delay that should be applied before tweening animatable
 * properties.
 *
 * Since: 1.10
 */
void
clutter_actor_set_easing_delay (ClutterActor *self,
                                guint         msecs)
{
  ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_animation_info (self);

  if (info->cur_state == NULL)
    {
      g_warning ("You must call clutter_actor_save_easing_state() prior "
                 "to calling clutter_actor_set_easing_delay().");
      return;
    }

  if (info->cur_state->easing_delay != msecs)
    info->cur_state->easing_delay = msecs;
}

/**
 * clutter_actor_get_easing_delay:
 * @self: a #ClutterActor
 *
 * Retrieves the delay that should be applied when tweening animatable
 * properties.
 *
 * Return value: a delay, in milliseconds
 *
 * Since: 1.10
 */
guint
clutter_actor_get_easing_delay (ClutterActor *self)
{
  const ClutterAnimationInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), 0);

  info = _clutter_actor_get_animation_info_or_defaults (self);

  if (info->cur_state != NULL)
    return info->cur_state->easing_delay;

  return 0;
}

/**
 * clutter_actor_get_transition:
 * @self: a #ClutterActor
 * @name: the name of the transition
 *
 * Retrieves the #ClutterTransition of a #ClutterActor by using the
 * transition @name.
 *
 * Transitions created for animatable properties use the name of the
 * property itself, for instance the code below:
 *
 * |[
 *   clutter_actor_set_easing_duration (actor, 1000);
 *   clutter_actor_set_rotation (actor, CLUTTER_Y_AXIS, 360.0, x, y, z);
 *
 *   transition = clutter_actor_get_transition (actor, "rotation-angle-y");
 *   g_signal_connect (transition, "completed",
 *                     G_CALLBACK (on_transition_complete),
 *                     actor);
 * ]|
 *
 * will call the <function>on_transition_complete</function> callback when
 * the transition is complete.
 *
 * Return value: (transfer none): a #ClutterTransition, or %NULL is none
 *   was found to match the passed name; the returned instance is owned
 *   by Clutter and it should not be freed
 *
 * Since: 1.10
 */
ClutterTransition *
clutter_actor_get_transition (ClutterActor *self,
                              const char   *name)
{
  TransitionClosure *clos;
  const ClutterAnimationInfo *info;

  g_return_val_if_fail (CLUTTER_IS_ACTOR (self), NULL);
  g_return_val_if_fail (name != NULL, NULL);

  info = _clutter_actor_get_animation_info_or_defaults (self);
  if (info->transitions == NULL)
    return NULL;

  clos = g_hash_table_lookup (info->transitions, name);
  if (clos == NULL)
    return NULL;

  return clos->transition;
}

/**
 * clutter_actor_save_easing_state:
 * @self: a #ClutterActor
 *
 * Saves the current easing state for animatable properties, and creates
 * a new state with the default values for easing mode and duration.
 *
 * Since: 1.10
 */
void
clutter_actor_save_easing_state (ClutterActor *self)
{
  ClutterAnimationInfo *info;
  AState new_state;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_animation_info (self);

  if (info->states == NULL)
    info->states = g_array_new (FALSE, FALSE, sizeof (AState));

  new_state.easing_mode = CLUTTER_EASE_OUT_CUBIC;
  new_state.easing_duration = 250;
  new_state.easing_delay = 0;

  g_array_append_val (info->states, new_state);

  info->cur_state = &g_array_index (info->states, AState, info->states->len - 1);
}

/**
 * clutter_actor_restore_easing_state:
 * @self: a #ClutterActor
 *
 * Restores the easing state as it was prior to a call to
 * clutter_actor_save_easing_state().
 *
 * Since: 1.10
 */
void
clutter_actor_restore_easing_state (ClutterActor *self)
{
  ClutterAnimationInfo *info;

  g_return_if_fail (CLUTTER_IS_ACTOR (self));

  info = _clutter_actor_get_animation_info (self);

  if (info->states == NULL)
    {
      g_critical ("The function clutter_actor_restore_easing_state() has "
                  "called without a previous call to "
                  "clutter_actor_save_easing_state().");
      return;
    }

  g_array_remove_index (info->states, info->states->len - 1);

  if (info->states->len > 0)
    info->cur_state = &g_array_index (info->states, AState, info->states->len - 1);
  else
    {
      g_array_unref (info->states);
      info->states = NULL;
      info->cur_state = NULL;
    }
}
