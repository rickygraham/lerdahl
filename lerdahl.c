/*
 
[lerdahl] v. 1 (2015) for Pd written by Ricky Graham <ricky@rickygraham.net>
 
Based on pitch space abstractions for melodic structures by F. Lerdahl (2001; 2007)
Closure: s2 >= s1
Melodic Tension: s2 / s1 * 1/abspcd^2 in Lerdahl 2001
Ratio of Asymmetrical Attraction: s2/s1 / s1/s2 in Lerdahl 2001, p. 167 - 168
Perceptual Fusion of Ratio of Asymmetrical Attraction and PCD: ROAA *  1/n^2 - p. 168

*/


#include "m_pd.h"
#include <math.h>
#include <stdlib.h>

static char *version = "[lerdahl] v. 1 (2015) for Pd written by Ricky Graham <ricky@rickygraham.net>";

static t_class *lerdahl_class;

typedef struct _lerdahl {
    
    t_object bs;
    
    t_int lastpitch;
    t_int lastbs; //for s2 - s1
    t_int offset; //offset for pc 0;
    
    t_outlet  *pclass;
    t_outlet  *basic;
    t_outlet  *closure;
    t_outlet  *tension;
    t_outlet  *ratio;
    t_outlet  *ratio_pcd;
    t_outlet  *pcd;
    t_outlet  *space;
    t_outlet  *everything;
    
    int modes[7]; //store modal structure
    
    t_symbol test;
    
} t_lerdahl;



void lerdahl_float(t_lerdahl *x, t_floatarg f1) {
    
    int lbs = x->lastbs; //store last bs
    int interval, pitch = 0;
    float j, k = 1; //j is abs pcd; k is 1 / j.
  
    pitch = (t_int)f1;
    
    /* Keep input within MIDI range */
    if(f1 < 1) pitch = 0;
    if(f1 > 127) pitch = 0;
 
    //post("\nlastp: %d", x->lastpitch);
    
    interval = pitch - x->lastpitch;
    x->lastpitch = pitch;
    
    //post("currentp: %d", pitch);
    //post("pcd: %d", interval);
   
    j = abs(interval);
    
    if(j == 0) { //if pcd is 0 then j = 1 / produces same desired effect; avoids divide by zero
        j = 1;
        } else {
        j = abs(interval);
        }
    //post("abs:%f", j);
    
        k = 1 / j;

    //post("lastbs: %d", lbs);
    
    char buf[10];
    char* spaces[4] = {"octave", "triadic", "diatonic", "chromatic"};
    
    int pc = (pitch - x->offset) % 12; //add offset, set by creation arg and / or right inlet
    
    int pclass = pc; //to output pitch class
    
    /* Octave Space */
      if(pc == x->modes[0]){
          //post("%d", mode[0]);
        pc = 4;
        sprintf(buf, "%s", spaces[0]); //stores final string in 'buf'
        //post("%s", spaces[c], r);
   
    /* Triadic Space */
    } else if((pc == x->modes[2] || (pc == x->modes[4]))) {
        pc = 3;
        sprintf(buf, "%s", spaces[1]);
    
    /* Diatonic Space */
    } else if((pc == x->modes[1] || (pc == x->modes[3]) || (pc == x->modes[5]) || (pc == x->modes[6]))) { //diatonic space
        pc = 2;
        sprintf(buf, "%s", spaces[2]);
        
    /* Chromatic Space */
    } else {
        pc = 1;
        sprintf(buf, "%s", spaces[3]);
    }
    
    float s1 = lbs;
    float s2 = pc;
    //post("currentbs: %d", s2);
  
    /* Tension, Ratios, etc. */
    float tension = (s2 / s1) * k;
    float roa_f = (s2 / s1) / (s1 / s2);
    float roa_pcd = roa_f * k;
    
    float clo;
    
    if(s2 >= s1) {
        clo = 1.0;
    } else {
        clo = 0.0;
        }
    //post("current pc set: %d %d %d %d %d %d %d", x->modes[0],x->modes[1],x->modes[2],x->modes[3], x->modes[4],x->modes[5],x->modes[6]);
    //post("cbs: %f, pbs: %f, pcd: %f ", s2, s1, k); //output as list - version 2
    
    x->lastbs = pc; //store last bs
    
    outlet_float(x->pclass, pclass); //pitch class
    outlet_float(x->basic, pc); //basic space
    outlet_float(x->closure, clo);
    outlet_float(x->tension, tension);
    outlet_float(x->ratio, roa_f);
    outlet_float(x->ratio_pcd, roa_pcd);
    outlet_float(x->pcd, interval); //interval from last note
    outlet_symbol(x->space, gensym(buf)); //space name
    //outlet_list - verison 2
}


void lerdahl_ionian(t_lerdahl *x) {
    
    int ionian[7] = {0, 2, 4, 5, 7, 9, 11};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=ionian[i];
    }
}

void lerdahl_dorian(t_lerdahl *x) {
    
    int dorian[7] = {0, 2, 3, 5, 7, 8, 10};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=dorian[i];
    }
}

void lerdahl_phrygian(t_lerdahl *x) {
    
    int phrygian[7] = {0, 1, 3, 5, 7, 8, 10};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=phrygian[i];
    }
}

void lerdahl_lydian(t_lerdahl *x) {
    
    int lydian[7] = {0, 2, 4, 6, 7, 9, 11};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=lydian[i];
    }
}

void lerdahl_mixolydian(t_lerdahl *x) {
    
    int mixolydian[7] = {0, 2, 4, 5, 7, 9, 10};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=mixolydian[i];
    }
}

void lerdahl_aeolian(t_lerdahl *x) {
    
    int aeolian[7] = {0, 2, 3, 5, 7, 8, 10};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=aeolian[i];
    }
}

void lerdahl_locrian(t_lerdahl *x) {
    
    int locrian[7] = {0, 1, 3, 5, 6, 8, 11};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=locrian[i];
    }
}


void lerdahl_offset(t_lerdahl *x, t_floatarg pc){
    int off = (int)pc;
    post("pczoffset: %d", off);
    x->offset = pc; //update pc
}



/* Constructor - Creation of Inlets and Outlets */
void *lerdahl_new(t_floatarg f) //f = pc 0; pass symbol to determine type of pitch class space (e.g., ionian)
{
    t_lerdahl *x = (t_lerdahl *)pd_new(lerdahl_class);
    int os = (int)f;
    post("pczoffset: %d", os);
    
    int ionian[7] = {0, 2, 4, 5, 7, 9, 11};
    for (int i = 0; i < 7; i++) {
        x->modes[i]=ionian[i];
    }
   
    x->offset = os; //user-defined pc zero
    x->lastbs = 1; //avoid initial inf
    
    //floatinlet_new(&x->bs, &x->in); //passive inlet
    x->pclass = outlet_new(&x->bs, gensym("float"));
    x->basic = outlet_new(&x->bs, gensym("float"));
    x->closure = outlet_new(&x->bs, gensym("float"));
    x->tension = outlet_new(&x->bs, gensym("float"));
    x->ratio = outlet_new(&x->bs, gensym("float"));
    x->ratio_pcd = outlet_new(&x->bs, gensym("float"));
    x->pcd = outlet_new(&x->bs, gensym("float"));
    x->space = outlet_new(&x->bs, gensym("symbol"));
    //x->everything = outlet_new(&x->bs, gensym("list")); - version 2
    return (void *)x;
}


/* Setup */
void lerdahl_setup(void)
{
    lerdahl_class = class_new(gensym("lerdahl"),
                              (t_newmethod)lerdahl_new,
                              0, sizeof(t_lerdahl),
                              CLASS_DEFAULT,
                              A_DEFFLOAT, 0); //pass pc 0
    class_addfloat(lerdahl_class, lerdahl_float); //float input for "lerdahl_float" method
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_offset, gensym("offset"), //for updating pitch class
                    A_DEFFLOAT, 0);
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_ionian, gensym("ionian"), 0);
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_dorian, gensym("dorian"), 0);
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_phrygian, gensym("phrygian"), 0);
   
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_lydian, gensym("lydian"), 0);
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_mixolydian, gensym("mixolydian"), 0);
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_aeolian, gensym("aeolian"), 0);
    
    class_addmethod(lerdahl_class,
                    (t_method)lerdahl_locrian, gensym("locrian"), 0);
    
    class_addcreator((t_newmethod)lerdahl_new, gensym("ler"), 0); // call object as "ler"
   
    class_sethelpsymbol(lerdahl_class, gensym("lerdahl-help.pd"));
    post(version);
    
}

