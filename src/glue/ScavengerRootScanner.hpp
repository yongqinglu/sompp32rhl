/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef SCAVENGERROOTSCANNER_HPP_
#define SCAVENGERROOTSCANNER_HPP_

#include "omr.h"
#include "omrcfg.h"
#include "omrExampleVM.hpp"
#include "omrhashtable.h"

#include "Base.hpp"
#include "EnvironmentStandard.hpp"
#include "ForwardedHeader.hpp"
#include "Scavenger.hpp"
#include "SublistFragment.hpp"

#if defined(OMR_GC_MODRON_SCAVENGER)

class MM_ScavengerRootScanner : public MM_Base
{
	/*
	 * Member data and types
	 */
private:
	MM_Scavenger *_scavenger;

protected:
public:

	/*
	 * Member functions
	 */
private:
protected:
public:
	MM_ScavengerRootScanner(MM_EnvironmentBase *env, MM_Scavenger *scavenger)
		: MM_Base()
		, _scavenger(scavenger)
	{
	};

	void
	scavengeRememberedSet(MM_EnvironmentStandard *env)
	{
		MM_SublistFragment::flush((J9VMGC_SublistFragment*)&env->_scavengerRememberedSet);
		_scavenger->scavengeRememberedSet(env);
	}

	void
	pruneRememberedSet(MM_EnvironmentStandard *env)
	{
		_scavenger->pruneRememberedSet(env);
	}

	void
	scanRoots(MM_EnvironmentBase *env)
	{
		/* TODO: Consider serializing thread access so that each gc thread gets a portion of the roots. */
		OMR_VM_Example *omrVM = (OMR_VM_Example *)env->getOmrVM()->_language_vm;
		if (env->_currentTask->synchronizeGCThreadsAndReleaseSingleThread(env, UNIQUE_ID)) {
			J9HashTableState state;
			MM_EnvironmentStandard *envStd = MM_EnvironmentStandard::getEnvironment(env);
			if (NULL != omrVM->rootTable) {
				RootEntry *rootEntry = (RootEntry *)hashTableStartDo(omrVM->rootTable, &state);
				while (NULL != rootEntry) {
					if (NULL != rootEntry->rootPtr) {
						_scavenger->copyObjectSlot(envStd, (volatile omrobjectptr_t *) &rootEntry->rootPtr);
					}
					rootEntry = (RootEntry *)hashTableNextDo(&state);
				}
			}
			env->_currentTask->releaseSynchronizedGCThreads(env);
		}
	}
	
#if !defined(OMR_GC_CONCURRENT_SCAVENGER)
	void rescanThreadSlots(MM_EnvironmentStandard *env) { }
#endif

	void scanClearable(MM_EnvironmentBase *env)
	{
		OMR_VM_Example *omrVM = (OMR_VM_Example *)env->getOmrVM()->_language_vm;
		if (NULL != omrVM->objectTable) {
			if (env->_currentTask->synchronizeGCThreadsAndReleaseSingleThread(env, UNIQUE_ID)) {
				J9HashTableState state;
				ObjectEntry *objectEntry = (ObjectEntry *)hashTableStartDo(omrVM->objectTable, &state);
				while (NULL != objectEntry) {
					if (_scavenger->isObjectInEvacuateMemory(objectEntry->objPtr)) {
						MM_ForwardedHeader fwdHeader(objectEntry->objPtr, OMR_OBJECT_METADATA_SLOT_OFFSET);
						if (fwdHeader.isForwardedPointer()) {
							omrobjectptr_t fwdPtr = fwdHeader.getForwardedObject();
							objectEntry->objPtr = fwdPtr;
						} else {
							hashTableDoRemove(&state);
						}
					}
					objectEntry = (ObjectEntry *)hashTableNextDo(&state);
				}
			}
			env->_currentTask->releaseSynchronizedGCThreads(env);
		}
	}

	void flush(MM_EnvironmentStandard *env) { }
};

#endif /* defined(OMR_GC_MODRON_SCAVENGER) */
#endif /* SCAVENGERROOTSCANNER_HPP_ */
