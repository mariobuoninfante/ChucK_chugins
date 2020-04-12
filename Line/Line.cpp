/*
 * Wrap UGen - wraps audio signals
 * based on Perry Cook's Envelope
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

CK_DLL_CTOR(line_ctor);
CK_DLL_DTOR(line_dtor);

CK_DLL_MFUN(line_setTarget);
CK_DLL_MFUN(line_getTarget);

CK_DLL_MFUN(line_setTime);
CK_DLL_MFUN(line_getTime);

CK_DLL_TICK(line_tick);

t_CKINT line_data_offset = 0;


class Line
{
public:
    Line( t_CKFLOAT fs )
    {
        SR = fs;
        target = 0.0;
        value = 0.0;
        rate = 0.001;
        m_target = 1.0;
        m_time = m_target / (rate * SR);
        state = 0;
    }

    SAMPLE tick( SAMPLE in )
    {
        if (state) 
        {
            if (target > value) 
            {
                value += rate;
                if (value >= target) 
                {
                    value = target;
                    state = 0;
                }
            }
            else 
            {
                value -= rate;
                if (value <= target) 
                {
                    value = target;
                    state = 0;
                }
            }
        }
        return value;
    }

    t_CKFLOAT setTarget( t_CKFLOAT x )
    {
        target = m_target = x;
        if (value != target) state = 1;

        // set time
        setTime( m_time );

        return x;
    }

    t_CKFLOAT setTime( t_CKFLOAT x )
    {
        // set time in seconds
        if (x < 0.0) 
        {
            printf("[chuck](via Line): negative times not allowed ... correcting!\n");
            x = -x;
        }
        
        if( x == 0.0 )
            rate = __FLT_MAX__;
        else
            rate = (target - value) / (x * SR);

        // rate
        if( rate < 0 ) rate = -rate;

        // should >= 0
        m_time = x;

        return x;
    }

    t_CKFLOAT getTarget() { return target; }

    t_CKFLOAT getTime() { return m_time; }

    
private:
    // instance data
    t_CKFLOAT   SR;
    t_CKFLOAT   value;
    t_CKFLOAT   rate;
    t_CKFLOAT   target;
    t_CKFLOAT   m_target;
    t_CKFLOAT   m_time;
    t_CKINT     state;
};


CK_DLL_QUERY( Line )
{
    QUERY->setname(QUERY, "Line");
    

    QUERY->begin_class(QUERY, "Line", "UGen");


    QUERY->add_ctor(QUERY, line_ctor);
    QUERY->add_dtor(QUERY, line_dtor);
    

    QUERY->add_ugen_func(QUERY, line_tick, NULL, 1, 1);
    

    QUERY->add_mfun(QUERY, line_setTarget, "float", "target");
    QUERY->add_arg(QUERY, "float", "arg");

    QUERY->add_mfun(QUERY, line_setTime, "float", "time");
    QUERY->add_arg(QUERY, "float", "arg");


    QUERY->add_mfun(QUERY, line_getTarget, "float", "target");

    QUERY->add_mfun(QUERY, line_getTime, "float", "time");


    line_data_offset = QUERY->add_mvar(QUERY, "int", "@l_data", false);

    QUERY->end_class(QUERY);

    return TRUE;
}


// implementation for the constructor
CK_DLL_CTOR(line_ctor)
{
    // get the offset where we'll store our internal c++ class pointer
    OBJ_MEMBER_INT(SELF, line_data_offset) = 0;
    
    // instantiate our internal c++ class representation
    Line * l_obj = new Line(API->vm->get_srate(API, SHRED));
    
    // store the pointer in the ChucK object member
    OBJ_MEMBER_INT(SELF, line_data_offset) = (t_CKINT) l_obj;
}


// implementation for the destructor
CK_DLL_DTOR(line_dtor)
{
    // get our c++ class pointer
    Line * l_obj = (Line *) OBJ_MEMBER_INT(SELF, line_data_offset);
    // check it
    if( l_obj )
    {
        // clean up
        delete l_obj;
        OBJ_MEMBER_INT(SELF, line_data_offset) = 0;
        l_obj = NULL;
    }
}


// implementation for tick function
CK_DLL_TICK(line_tick)
{
    // get our c++ class pointer
    Line * l_obj = (Line *) OBJ_MEMBER_INT(SELF, line_data_offset);
 
    // invoke our tick function; store in the magical out variable
    if(l_obj) *out = l_obj->tick(in);

    return TRUE;
}


CK_DLL_MFUN(line_setTarget)
{
    // get our c++ class pointer
    Line * l_obj = (Line *) OBJ_MEMBER_INT(SELF, line_data_offset);
    // set the return value
    RETURN->v_float = l_obj->setTarget(GET_NEXT_FLOAT(ARGS));
}

CK_DLL_MFUN(line_setTime)
{
    // get our c++ class pointer
    Line * l_obj = (Line *) OBJ_MEMBER_INT(SELF, line_data_offset);
    // set the return value
    RETURN->v_float = l_obj->setTime(GET_NEXT_FLOAT(ARGS));
}

CK_DLL_MFUN(line_getTarget)
{
    // get our c++ class pointer
    Line * l_obj = (Line *) OBJ_MEMBER_INT(SELF, line_data_offset);
    // set the return value
    RETURN->v_float = l_obj->getTarget();
}

CK_DLL_MFUN(line_getTime)
{
    // get our c++ class pointer
    Line * l_obj = (Line *) OBJ_MEMBER_INT(SELF, line_data_offset);
    // set the return value
    RETURN->v_float = l_obj->getTime();
}
