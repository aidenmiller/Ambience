/**
 *  @file       Light.cpp
 *  @author     CS 3307 - Team 13
 *  @date       10/7/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Hue Light Application Bridge class to store a Light object
 *
 *  @section    DESCRIPTION
 *
 *              This class stores a Light object that is generated when viewing a Bridge object
 */

#include "Light.h"

/**
 *   @brief  Light constructor
 *
 *   @param  type is the type of light
 *   @param  name is the name of the light
 *   @param  modelid is the modelid of the light
 *   @param  swversion is the swversion of the light
 *   @param  uniqueid is the uniqueid of the light
 *
 */
Light::Light(string type, string name, string modelid,
      string swversion, string uniqueid, struct state_t state) {
    type_ = type;
    name_ = name;
    modelid_ = modelid;
    swversion_ = swversion;
    uniqueid_ = uniqueid;
    state_ = state;
}

/**
 *   @brief  Light destructor
 *
 */
Light::~Light() {
    
}
