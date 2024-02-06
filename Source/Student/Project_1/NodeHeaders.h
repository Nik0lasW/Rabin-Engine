#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes


// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_RepeatTwoTimes.h"
#include "Decorator/D_RepeatUntilSuccess.h"

// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"
#include "Leaf/L_PlaySound.h"

// Student Leaf Nodes
#include "Leaf/L_MoveToAvailablePresent.h"
#include "Leaf/L_MoveToTree.h"
#include "Leaf/L_CheckNearTree.h"
#include "Leaf/L_CheckNearPlayer.h"
#include "Leaf/L_MoveToPlayer.h"
#include "Leaf/L_CheckNearGrinch.h"
#include "Leaf/L_MoveToGrinch.h"
#include "Leaf/L_MoveToCorners.h"
#include "Leaf/L_CheckNearCop.h"
#include "Leaf/L_Stun.h"