#ifndef __SIN_H__

static int getsin(int angle);

static const uint8_t table_sin[] = {
	0,
	12,
	25,
	37,
	49,
	62,
	74,
	86,
	97,
	109,
	120,
	131,
	142,
	152,
	162,
	171,
	181,
	189,
	197,
	205,
	212,
	219,
	225,
	231,
	236,
	241,
	244,
	248,
	251,
	253,
	254,
	255,
};

static int getsin(int angle)
{
    /*
    if(angle<90)
        return table_sin[angle]/4+64;
    if(angle==90)
        return 127;
    if(angle<=180)
        return table_sin[180-angle]/4+64;
    if(angle<270)
        return 64-table_sin[angle-180]/4;
    if(angle==270)
        return 0;
    if(angle<360)
        return 64-table_sin[360-angle]/4;
    */
    angle &= 127;
    if(angle<32)
        return table_sin[angle]/4+64;
    if(angle==32)
        return 127;
    if(angle<=64)
        return table_sin[64-angle]/4+64;
    if(angle<96)
        return 64-table_sin[angle-64]/4;
    if(angle==96)
        return 0;
    //if(angle<128)
        return 64-table_sin[128-angle]/4;
}

#endif /* __SIN_H__ */

