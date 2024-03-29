/*!***************************************************************************************
\file       Task.hpp
\author     Michael-Paul Moore
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This is the interface for the class API task. Also includes some useful defines
        and typedefs. Don't forget Tasks reschedule by default.
*****************************************************************************************/

#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <chrono>
#include <DataTypes.hpp>

//========Types=========================================================================//
typedef          char thread_number; //!< unit for thread numbering

//========Defines=======================================================================//
#define MAIN_THREAD_AFFINITY thread_number(-1) /*!< Flag that affinity should be main
                                               * thread. DONT CHANGE VALUE, TODO(MP):
                                               * remove this DONT CHANGE VALUE dependency
                                               * as best I can, also check if done. */
#define NO_AFFINITY          thread_number(-2) //!< No affinity used.
#define NOT_SET_AFFINITY     thread_number(-3) //!< Affinity not set but should be used.

////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: task
\brief   Tasks is to be inherited, contains data and needed functionality for a single
         task.
*****************************************************************************************/
class task
{
	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////
	public:

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Constructor                                                                   //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  This just for giving an example of a function format.
		\param p_affinity - if you want this task to remain on the same processor
		\param reschedule - does this reschedule when finished
		*****************************************************************************************/
		explicit task(const thread_number & p_affinity = NO_AFFINITY, bool reschedule = true);

		/*!***************************************************************************************
		\brief  This just for giving an example of a function format.
		\param p_affinity - if you want this task to remain on the same processor
		\param reschedule - does this reschedule when finished
		*****************************************************************************************/
		explicit task(const fps& max_fps, const thread_number & p_affinity = NO_AFFINITY);

		//======================================================================================//
		//        Getters & Setters                                                            ///
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Gets task thread affinity.
		\return Thread affinity.
		*****************************************************************************************/
		thread_number getAffinity() const;

		/*!***************************************************************************************
		\brief  Set thread affinity
		\param affinity - affinity to set
		*****************************************************************************************/
		void setAffinity(thread_number affinity);

		/*!***************************************************************************************
		\brief  Returns if this task is marked for reschedule.
		\return Result if it is marked for rescheduled.
		*****************************************************************************************/
		bool isRescheduled() const;

		/*!***************************************************************************************
		\brief  Marks task to be unscheduled.
		*****************************************************************************************/
		void markUnschedule();

		/*!***************************************************************************************
		\brief  Marks task to be rescheduled.
		*****************************************************************************************/
		void markReschedule();

		/*!***************************************************************************************
		\brief  Returns if this task is removed.
		\return Result if it is removed.
		*****************************************************************************************/
		bool isRemoved() const;

		/*!***************************************************************************************
		\brief  Marks task as added.
		*****************************************************************************************/
		void markAdded();

		/*!***************************************************************************************
		\brief  Marks task as removed.
		*****************************************************************************************/
		void markRemoved();

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Checks if this task is ready to run. Also if it is aged push forward the timer.
		\return True if it is at or after time, false if it is not ready yet.
		*****************************************************************************************/
		bool aged();

		////==================================================================================////
		////      Pure-Virtual                                                                ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  What does this task do on it's update.
		*****************************************************************************************/
		virtual void onUpdate() = 0;

		////==================================================================================////
		////      Virtual                                                                     ////
		////==================================================================================////

		//======================================================================================//
		//        Destructor                                                                   ///
		//======================================================================================//
		virtual ~task() = default;

		///////============================================================================///////
		///////   Private                                                                  ///////
		///////============================================================================///////
		private:

			//////==============================================================================//////
			//////    Non-Static                                                                //////
			//////==============================================================================//////

			/////================================================================================/////
			/////     Data                                                                       /////
			/////================================================================================/////

			////==================================================================================////
			////      Configurable                                                                ////
			////==================================================================================////

			thread_number m_afinity    = NO_AFFINITY; //!< The thread affinity for the task.
			bool          m_reschedule = true;        /*!< Reschedules when complete. TODO(MP): Remove
													  * this default */
			const std::chrono::duration<int, std::nano>          m_max_frq = {}; //!< normal frequency
			      std::chrono::high_resolution_clock::time_point m_nxt_tk  = {}; //!< time for next tick

			////==================================================================================////
			////      Non-Configurable                                                            ////
			////==================================================================================////

			bool m_removed = false; //!< If this task has been removed from the job manager.
};
