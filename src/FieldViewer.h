/*******************************************************************************
 FieldViewer: Dynamics Toolset application class.
 Copyright (c) 2006-2008 Jordan Van Aalsburg

 This file is part of the Dynamics Toolset.

 The Dynamics Toolset is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option) any
 later version.

 The Dynamics Toolset is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License
 along with the Dynamics Toolset. If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/
#ifndef FIELD_VIEWER_H
#define FIELD_VIEWER_H

// Vrui includes
//
#include <Vrui/Application.h>
#include <GL/GLObject.h>
#include <IO/OpenFile.h>

// STL includes
//
#include <exception>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Experiment.h"
#include "Factory.h"
#include "Tools/AbstractDynamicsTool.h"
#include "PositionDialog.h"
#include "FrameRateDialog.h"
#include "ExperimentDialog.h"

// External includes
//
#include "VruiStreamManip.h"

// font rendering
#include <FTGL/ftgl.h>



/** The main Vrui application class.
 */
class Viewer: public Vrui::Application, public GLObject
{
   public:
      /* Embedded classes */

      typedef AbstractDynamicsTool ADT;
      typedef std::vector<ADT*> ToolList;
      typedef std::vector<GLMotif::ToggleButton*> ToggleArray;
      typedef std::vector<CaveDialog*> DialogArray;

      /* Interface */
      Viewer(int &argc, char** argv, char** appDefaults);
      virtual ~Viewer();

      void initContext(GLContextData& contextData) const;
      virtual void display(GLContextData& contextData) const;
      virtual void frame();

      /** Update the tool toggle buttons.
       *
       *  Iterates over tools and sets the toggle state to false if the tool
       *  is current disabled.
       */
      void updateToolToggles();

      /** Switch the options dialog to the one associated with the current tool.
       *
       *  Only one tool options dialog is displayed at a time. When a tool is
       *  changed the dialog is updated to reflect this change.
       */
      void updateCurrentOptionsDialog();

      /* Callbacks */
      virtual void toolCreationCallback(Vrui::ToolManager::ToolCreationCallbackData* cbData);
      virtual void toolDestructionCallback(Vrui::ToolManager::ToolDestructionCallbackData* cbData);
      void mainMenuTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData *cbData);
      void dynamicsMenuCallback(GLMotif::ToggleButton::ValueChangedCallbackData *cbData);
      void toolsMenuCallback(GLMotif::ToggleButton::ValueChangedCallbackData *cbData);
      void resetNavigationCallback(Misc::CallbackData* cbData);

      void setExperiment(std::string, bool updateToggle=true);

   private:
      ToolList tools; ///< Array of all tools currently being used.
      Experiment<Scalar> *experiment;

      FrameRateDialog* frameRateDialog; ///< Dialog for throttling the frame rate.
      PositionDialog* positionDialog; ///< Dialog for displaying cursor position.
      ExperimentDialog* experimentDialog; ///< Parameter dialog associated with current experiment
      CaveDialog* currentOptionsDialog; ///< Options dialog associated with the current tool.
      DialogArray optionsDialogs; ///< Array of all tool options dialogs.

      /* User interface methods (see FieldViewer_ui.cpp for implementation) */
      GLMotif::PopupMenu* mainMenu;
      GLMotif::PopupMenu* createMainMenu();
      GLMotif::Popup* createDynamicsTogglesMenu();
      GLMotif::Popup* createToolsTogglesMenu();
      GLMotif::ToggleButton* showParameterDialogToggle;

      ToggleArray toolsToggleButtons;
      ToggleArray dynamicsToggleButtons;

      /* So that we can make sure only one toolbox is created (and deleted) */
      ToolBox::ToolBox* toolbox;
      std::map<std::string, AbstractDynamicsTool*> toolmap;

      typedef std::list<void*> DLList;
      DLList dl_list; ///< Dynamic library (plugin) list.
      std::vector<std::string> experiment_names; ///< Names of all experiments (obtained from plugins).

      double elapsedTime; // Cummulative time between frames (that is reset frequently)
      double absoluteTime;

      /* Output streams */
      master::filter masterout;
      node::filter nodeout;
      debug::filter debugout;

      bool showingLogo; // whether the logo is presently being showed
      bool firstTime; // for the first time the logo is drawn
      bool startLogo;

      /* Internal methods */

      /** Internal method for faking radio-button behavior.
       *
       * Iterates through array of toggle buttons and sets the state to
       * false unless the toggle button name matches the one given.
       *
       * \param toggles Array of toggle buttons.
       * \param name Name of toggle to set to true (all others will be set to false).
       */
      void setRadioToggles(ToggleArray& toggles, const std::string& name);

      /** Internal method for loading plugins (dlls).
       *
       * Searches the plugins directory for dynamic libraries. Each library
       * that is found is then loaded into memory. An exception is thrown
       * if a library fails to load.
       *
       * \return An array of the names of all plugins.
       */
      std::vector<std::string> loadPlugins() throw(std::runtime_error);

      virtual void drawLogo(GLContextData& contextData) const;
      virtual void stepLogo();
      virtual void renderTools(DTS::DataItem*) const;
      virtual void resetView(); 
      virtual bool loadViewpointFile(IO::Directory& directory,const char* viewpointFileName);
      void beginLogo();
      void endLogo();
};

#endif
