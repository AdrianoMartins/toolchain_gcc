/* AbstractColorChooserPanel.java --
   Copyright (C) 2002, 2004, 2005  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


package javax.swing.colorchooser;

import java.awt.Color;
import java.awt.Graphics;

import javax.swing.Icon;
import javax.swing.JColorChooser;
import javax.swing.JPanel;

/**
 * AbstractColorChooserPanel
 *
 * @author Andrew Selkirk
 * @version 1.0
 */
public abstract class AbstractColorChooserPanel extends JPanel
{
  /** DOCUMENT ME! */
  private static final long serialVersionUID = -977469671210173863L;

  /** The chooser associated with this panel. */
  private JColorChooser chooser;

  /**
   * This is the constructor for the AbstractColorChooserPanel.
   */
  public AbstractColorChooserPanel()
  {
    // Nothing to do here.
  }

  /**
   * This method returns the name displayed in the tab for this chooser panel.
   *
   * @return The name displayed in the JTabbedPane's tabs.
   */
  public abstract String getDisplayName();

  /**
   * Returns the key code for the mnemonic for this panel.  This method returns
   * zero to indicate no mnemonic, subclasses can override this.
   *
   * @return <code>0</code>, to indicate no mnemonic key code.
   *
   * @see #getDisplayedMnemonicIndex()
   * @since 1.4
   */
  public int getMnemonic()
  {
    return 0;
  }

  /**
   * Returns the index of the character in the display name that is the
   * mnemonic.  This method returns <code>-1</code> to indicate no mnemonic,
   * subclasses can override.
   *
   * @return <code>-1</code>, to indicate no mnemonic.
   *
   * @see #getDisplayName()
   * @see #getMnemonic()
   * @since 1.4
   */
  public int getDisplayedMnemonicIndex()
  {
    return -1;
  }

  /**
   * This method updates the chooser panel when the JColorChooser's color has
   * changed.
   */
  public abstract void updateChooser();

  /**
   * This method constructs and does any initialization necessary for the
   * chooser panel.
   */
  protected abstract void buildChooser();

  /**
   * This method sets the small icon used in the JTabbedPane for this chooser
   * panel.
   *
   * @return The small icon used in the JTabbedPane.
   */
  public abstract Icon getSmallDisplayIcon();

  /**
   * This method sets the large icon useed in the jTabbedPane for this chooser
   * panel.
   *
   * @return The large icon.
   */
  public abstract Icon getLargeDisplayIcon();

  /**
   * This method installs the chooser panel for the given JColorChooser.
   *
   * @param chooser The JColorChooser that will have this panel installed.
   */
  public void installChooserPanel(JColorChooser chooser)
  {
    this.chooser = chooser;
    buildChooser();
  } // installChooserPanel()

  /**
   * This method removes the chooser panel from the given JColorChooser and
   * does any necessary clean up for the chooser panel.
   *
   * @param chooser The JColorChooser that is having this panel removed.
   */
  public void uninstallChooserPanel(JColorChooser chooser)
  {
    this.chooser = null;
  } // uninstallChooserPanel()

  /**
   * This method returns the ColorSelectionModel for the JColorChooser
   * associated with this chooser panel.
   *
   * @return The ColorSelectionModel for the JColorChooser associated with
   *         this chooser panel.
   */
  public ColorSelectionModel getColorSelectionModel()
  {
    if (chooser != null)
      return chooser.getSelectionModel();
    return null;
  } // getColorSelectionModel()

  /**
   * This method returns the current color stored in the model for this
   * chooser panel.
   *
   * @return The current color.
   */
  protected Color getColorFromModel()
  {
    if (chooser != null)
      return chooser.getColor();
    return null;
  } // getColorFromModel()

  /**
   * This method paints the chooser panel.
   *
   * @param graphics The Graphics object to paint with.
   */
  public void paint(Graphics graphics)
  {
    super.paint(graphics);
  } // paint()
} // AbstractColorChooserPanel
