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
 *   @param  lightData the Json object of a Light from the Hue API
 *
 */
Light::Light(int lightNum, Json::Object lightData) {
    lightnum_ = lightNum;
    name_ = lightData.get("name");
    type_ = lightData.get("type");
    modelid_ = lightData.get("modelid");
    
    Json::Object state = lightData.get("state");
    alert_ = state.get("alert");
    bri_ = state.get("bri");
    colormode_ = state.get("colormode");
    ct_ = state.get("ct");
    effect_ = state.get("effect");
    hue_ = state.get("hue");
    on_ = state.get("on");
    reachable_ = state.get("reachable");
    sat_ = state.get("sat");
    
    Json::Array xy = state.get("xy");
    x_ = xy[0];
    y_ = xy[1];
}

/**
 *   @brief  Light destructor
 *
 */
Light::~Light() {
    
}
