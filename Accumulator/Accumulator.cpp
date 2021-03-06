/*
 * Acuumulator UGen - "endless" Phasor
 * 
 * created to scan audio buffers (array)
 * it has a "scale" factor for buffers that accepts signals in a range 0-1
 * 
 * @author      Mario Buoninfante
 * @year        2020
*/


//-----------------------------------------------------------------------------
// Entaro ChucK Developer!
// This is a Chugin boilerplate, generated by chuginate!
//-----------------------------------------------------------------------------

// this should align with the correct versions of these ChucK files
#include "chuck_dl.h"
#include "chuck_def.h"

// general includes
#include <stdio.h>
#include <limits.h>

// declaration of chugin constructor
CK_DLL_CTOR(accumulator_ctor);
// declaration of chugin desctructor
CK_DLL_DTOR(accumulator_dtor);

// example of getter/setter
CK_DLL_MFUN(accumulator_setStep);
CK_DLL_MFUN(accumulator_setScale);
CK_DLL_MFUN(accumulator_getStep);
CK_DLL_MFUN(accumulator_getScale);


// for Chugins extending UGen, this is mono synthesis function for 1 sample
CK_DLL_TICK(accumulator_tick);

// this is a special offset reserved for Chugin internal data
t_CKINT accumulator_data_offset = 0;


// class definition for internal Chugin data
// (note: this isn't strictly necessary, but serves as example
// of one recommended approach)
class Accumulator
{
public:
    // constructor
    Accumulator( t_CKFLOAT fs)
    {
        m_step = 1;
        m_out = 0;
        m_prev_in = 0;
        m_scale = 0;
        m_scale_factor = 1;
    }

    // for Chugins extending UGen
    SAMPLE tick( SAMPLE in )
    {
        if (in < m_prev_in || in < 0)
        {
            m_out = 0;
        }
        else
        {
            m_out += m_step;
        }
        
        m_prev_in = in;


        // having this in place instead of setting "m_step = 1 / m_scale_factor" 
        // seems to be more computationally friendly
        if (m_scale)
            return m_out * m_scale_factor;
        else
           return m_out;
    }

    // set parameter example
    t_CKFLOAT setStep( t_CKFLOAT step )
    {
        m_step = step;
        return m_step;
    }

    t_CKFLOAT setScale( t_CKFLOAT scale_factor )
    {
        m_scale_factor = scale_factor;
        if (m_scale_factor != 0)
            m_scale = 1;
        else
            m_scale = 0;    
    
        return m_scale_factor;
    }

    // get parameter example
    t_CKFLOAT getStep() { return m_step; }
    
    t_CKFLOAT getScale() { return m_scale_factor; }

private:
    // instance data
    t_CKFLOAT m_step;
    t_CKFLOAT m_prev_in;
    t_CKFLOAT m_out;
    t_CKFLOAT m_scale_factor;
    t_CKINT m_scale;
};


// query function: chuck calls this when loading the Chugin
// NOTE: developer will need to modify this function to
// add additional functions to this Chugin
CK_DLL_QUERY( Accumulator )
{
    // hmm, don't change this...
    QUERY->setname(QUERY, "Accumulator");
    
    // begin the class definition
    // can change the second argument to extend a different ChucK class
    QUERY->begin_class(QUERY, "Accumulator", "UGen");

    // register the constructor (probably no need to change)
    QUERY->add_ctor(QUERY, accumulator_ctor);
    // register the destructor (probably no need to change)
    QUERY->add_dtor(QUERY, accumulator_dtor);
    
    // for UGen's only: add tick function
    QUERY->add_ugen_func(QUERY, accumulator_tick, NULL, 1, 1);
    
    // NOTE: if this is to be a UGen with more than 1 channel, 
    // e.g., a multichannel UGen -- will need to use add_ugen_funcf()
    // and declare a tickf function using CK_DLL_TICKF

    // example of adding setter method
    QUERY->add_mfun(QUERY, accumulator_setStep, "float", "setStep");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");

    // example of adding setter method
    QUERY->add_mfun(QUERY, accumulator_setScale, "float", "setScale");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");


    // example of adding getter method
    QUERY->add_mfun(QUERY, accumulator_getStep, "float", "getStep");
    
    // example of adding getter method
    QUERY->add_mfun(QUERY, accumulator_getScale, "float", "getScale");
    

    // this reserves a variable in the ChucK internal class to store 
    // referene to the c++ class we defined above
    accumulator_data_offset = QUERY->add_mvar(QUERY, "int", "@a_data", false);

    // end the class definition
    // IMPORTANT: this MUST be called!
    QUERY->end_class(QUERY);

    // wasn't that a breeze?
    return TRUE;
}


// implementation for the constructor
CK_DLL_CTOR(accumulator_ctor)
{
    // get the offset where we'll store our internal c++ class pointer
    OBJ_MEMBER_INT(SELF, accumulator_data_offset) = 0;
    
    // instantiate our internal c++ class representation
    Accumulator * a_obj = new Accumulator(API->vm->get_srate(API, SHRED));
    
    // store the pointer in the ChucK object member
    OBJ_MEMBER_INT(SELF, accumulator_data_offset) = (t_CKINT) a_obj;
}


// implementation for the destructor
CK_DLL_DTOR(accumulator_dtor)
{
    // get our c++ class pointer
    Accumulator * a_obj = (Accumulator *) OBJ_MEMBER_INT(SELF, accumulator_data_offset);
    // check it
    if( a_obj )
    {
        // clean up
        delete a_obj;
        OBJ_MEMBER_INT(SELF, accumulator_data_offset) = 0;
        a_obj = NULL;
    }
}


// implementation for tick function
CK_DLL_TICK(accumulator_tick)
{
    // get our c++ class pointer
    Accumulator * a_obj = (Accumulator *) OBJ_MEMBER_INT(SELF, accumulator_data_offset);
 
    // invoke our tick function; store in the magical out variable
    if(a_obj) *out = a_obj->tick(in);

    // yes
    return TRUE;
}


// example implementation for setter
CK_DLL_MFUN(accumulator_setStep)
{
    // get our c++ class pointer
    Accumulator * a_obj = (Accumulator *) OBJ_MEMBER_INT(SELF, accumulator_data_offset);
    // set the return value
    RETURN->v_float = a_obj->setStep(GET_NEXT_FLOAT(ARGS));
}

CK_DLL_MFUN(accumulator_setScale)
{
    // get our c++ class pointer
    Accumulator * a_obj = (Accumulator *) OBJ_MEMBER_INT(SELF, accumulator_data_offset);
    // set the return value
    RETURN->v_float = a_obj->setScale(GET_NEXT_FLOAT(ARGS));
}


// example implementation for getter
CK_DLL_MFUN(accumulator_getStep)
{
    // get our c++ class pointer
    Accumulator * a_obj = (Accumulator *) OBJ_MEMBER_INT(SELF, accumulator_data_offset);
    // set the return value
    RETURN->v_float = a_obj->getStep();
}


CK_DLL_MFUN(accumulator_getScale)
{
    // get our c++ class pointer
    Accumulator * a_obj = (Accumulator *) OBJ_MEMBER_INT(SELF, accumulator_data_offset);
    // set the return value
    RETURN->v_float = a_obj->getScale();
}
