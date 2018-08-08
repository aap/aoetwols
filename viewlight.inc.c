#define FLAT(s) (s == SLOPE_FLAT)
#define N_UP(s) (s == SLOPE_N_UP || s == SLOPE_N_UP2)
#define S_UP(s) (s == SLOPE_S_UP || s == SLOPE_S_UP2)
#define W_UP(s) (s == SLOPE_W_UP || s == SLOPE_W_UP2)
#define E_UP(s) (s == SLOPE_E_UP || s == SLOPE_E_UP2)
#define ANY_W_UP(s) (W_UP(s) || s == SLOPE_SW_UP || s == SLOPE_NW_UP)
#define ANY_E_UP(s) (E_UP(s) || s == SLOPE_SE_UP || s == SLOPE_NE_UP)
#define S_OR_W_UP(s) (S_UP(s) || W_UP(s) || s == SLOPE_SW_UP)
#define N_OR_W_UP(s) (N_UP(s) || W_UP(s) || s == SLOPE_NW_UP)
#define S_OR_E_UP(s) (S_UP(s) || E_UP(s) || s == SLOPE_SE_UP)
#define N_OR_E_UP(s) (N_UP(s) || E_UP(s) || s == SLOPE_NE_UP)
#define ANY_SW_UP(s) (s == SLOPE_SW_UP || s == SLOPE_SWE_UP || s == SLOPE_NSW_UP)
#define ANY_NW_UP(s) (s == SLOPE_NW_UP || s == SLOPE_NWE_UP || s == SLOPE_NSW_UP)
#define ANY_SE_UP(s) (s == SLOPE_SE_UP || s == SLOPE_SWE_UP || s == SLOPE_NSE_UP)
#define ANY_NE_UP(s) (s == SLOPE_NE_UP || s == SLOPE_NWE_UP || s == SLOPE_NSW_UP)

static int
ViewLightSlope_S_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 2;
	if(FLAT(slopes[TILE_S]))
		patterns[n++] = 18;
	if(FLAT(slopes[TILE_NE]) + FLAT(slopes[TILE_NW]) + FLAT(slopes[TILE_N]) > 1)
		patterns[n++] = 19;
	if(FLAT(slopes[TILE_NW]) || N_OR_W_UP(slopes[TILE_NW]))
		patterns[n++] = 8;
	else if(FLAT(slopes[TILE_W]))
		patterns[n++] = 26;
	if(!(FLAT(slopes[TILE_NE]) || N_OR_E_UP(slopes[TILE_NE]))){
		if(!FLAT(slopes[TILE_E]))
			patterns[n++] = 28;
	}else
		patterns[n++] = 7;
	if(slopes[TILE_NE] == SLOPE_SWE_UP || slopes[TILE_SE] == SLOPE_SWE_UP || FLAT(slopes[TILE_E]))
		patterns[n++] = 22;
	if(slopes[TILE_NW] == SLOPE_SWE_UP || slopes[TILE_SW] == SLOPE_SWE_UP || FLAT(slopes[TILE_W]))
		patterns[n++] = 21;
	return n;
}

static int
ViewLightSlope_N_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 2;
	if(FLAT(slopes[TILE_N]))
		patterns[n++] = 16;
	if(FLAT(slopes[TILE_SE]) + FLAT(slopes[TILE_SW]) + FLAT(slopes[TILE_S]) > 1)
		patterns[n++] = 17;
	if(FLAT(slopes[TILE_SW]) || S_OR_W_UP(slopes[TILE_SW]))
		patterns[n++] = 10;
	else if(FLAT(slopes[TILE_W]))
		patterns[n++] = 26;
	if(!(FLAT(slopes[TILE_SE]) || S_OR_E_UP(slopes[TILE_SE]))){
		if(!FLAT(slopes[TILE_E]))
			patterns[n++] = 28;
	}else
		patterns[n++] = 5;
	if(slopes[TILE_NW] == SLOPE_NWE_UP || slopes[TILE_SW] == SLOPE_NWE_UP || FLAT(slopes[TILE_W]))
		patterns[n++] = 21;
	if(slopes[TILE_NE] == SLOPE_NWE_UP || slopes[TILE_SE] == SLOPE_NWE_UP || FLAT(slopes[TILE_E]))
		patterns[n++] = 22;
	return n;
}

static int
ViewLightSlope_W_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 0;
	if(FLAT(slopes[TILE_W]))
		patterns[n++] = 12;
	if(FLAT(slopes[TILE_NE]) + FLAT(slopes[TILE_SE]) + FLAT(slopes[TILE_E]) > 1)
		patterns[n++] = 13;
	if(FLAT(slopes[TILE_NE]) || N_OR_E_UP(slopes[TILE_NE]))
		patterns[n++] = 7;
	else if(FLAT(slopes[TILE_N]))
		patterns[n++] = 34;
	if(!(FLAT(slopes[TILE_SE]) || S_OR_E_UP(slopes[TILE_SE]))){
		if(!FLAT(slopes[TILE_S]))
			patterns[n++] = 35;
	}else
		patterns[n++] = 5;
	return n;
}

static int
ViewLightSlope_E_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 1;
	if(FLAT(slopes[TILE_E]))
		patterns[n++] = 14;
	if(FLAT(slopes[TILE_NW]) + FLAT(slopes[TILE_SW]) + FLAT(slopes[TILE_W]) > 1)
		patterns[n++] = 15;
	if(FLAT(slopes[TILE_SW]) || S_OR_W_UP(slopes[TILE_SW]))
		patterns[n++] = 10;
	else if(FLAT(slopes[TILE_S]))
		patterns[n++] = 37;
	if(!(FLAT(slopes[TILE_NW]) || N_OR_W_UP(slopes[TILE_NW]))){
		if(!FLAT(slopes[TILE_N]))
			patterns[n++] = 36;
	}else
		patterns[n++] = 8;
	return n;
}

static int
ViewLightSlope_SW_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 0;
	if(FLAT(slopes[TILE_SW]) || ANY_NE_UP(slopes[TILE_SW]))
		patterns[n++] = 6;
	else{
		if(FLAT(slopes[TILE_W]))
			patterns[n++] = 12;
		if(FLAT(slopes[TILE_S]))
			patterns[n++] = 35;
	}
	if(FLAT(slopes[TILE_NE]) || N_OR_E_UP(slopes[TILE_NE]))
		patterns[n++] = 7;
	else{
		if(FLAT(slopes[TILE_N]) || S_UP(slopes[TILE_N]))
			patterns[n++] = 34;
		if(FLAT(slopes[TILE_E]) || E_UP(slopes[TILE_E]))
			patterns[n++] = 28;
	}
	if(S_UP(slopes[TILE_S]) && W_UP(slopes[TILE_SW]))
		patterns[n++] = 35;
	if(S_UP(slopes[TILE_SW]) && W_UP(slopes[TILE_W]))
		patterns[n++] = 12;
	if(slopes[TILE_NE] == SLOPE_SWE_UP || slopes[TILE_SE] == SLOPE_SWE_UP)
		patterns[n++] = 22;
	if(S_UP(slopes[TILE_NW]))
		patterns[n++] = 23;
	if(FLAT(slopes[TILE_E]))
		patterns[n++] = 28;
	return n;
}

static int
ViewLightSlope_NW_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 0;
	if(FLAT(slopes[TILE_NW]) || ANY_SE_UP(slopes[TILE_NW]))
		patterns[n++] = 4;
	else{
		if(FLAT(slopes[TILE_W]))
			patterns[n++] = 12;
		if(FLAT(slopes[TILE_N]))
			patterns[n++] = 34;
	}
	if(FLAT(slopes[TILE_SE]) || S_OR_E_UP(slopes[TILE_SE]))
		patterns[n++] = 5;
	else{
		if(FLAT(slopes[TILE_S]) || S_UP(slopes[TILE_S]))
			patterns[n++] = 35;
		if(FLAT(slopes[TILE_E]) || E_UP(slopes[TILE_E]))
			patterns[n++] = 28;
	}
	if(N_UP(slopes[TILE_SW]))
		patterns[n++] = 23;
	if(FLAT(slopes[TILE_W]))
		patterns[n++] = 12;
	if(slopes[TILE_NE] == SLOPE_NWE_UP)
		patterns[n++] = 22;
	if(FLAT(slopes[TILE_E]))
		patterns[n++] = 28;
	if(FLAT(slopes[TILE_N]))
		patterns[n++] = 34;
	if(W_UP(slopes[TILE_W]) && N_UP(slopes[TILE_NW]))
		patterns[n++] = 12;
	if(W_UP(slopes[TILE_NW]) && N_UP(slopes[TILE_N]))
		patterns[n++] = 34;
	return n;
}

static int
ViewLightSlope_SE_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 1;
	if(FLAT(slopes[TILE_NW]) || N_OR_W_UP(slopes[TILE_NW]))
		patterns[n++] = 8;
	else{
		if(FLAT(slopes[TILE_W]) || W_UP(slopes[TILE_W]))
			patterns[n++] = 26;
		if(FLAT(slopes[TILE_N]) || N_UP(slopes[TILE_N]))
			patterns[n++] = 36;
	}
	if(FLAT(slopes[TILE_SE]) || ANY_NW_UP(slopes[TILE_SE]))
		patterns[n++] = 9;
	else{
		if(FLAT(slopes[TILE_E]))
			patterns[n++] = 14;
		if(FLAT(slopes[TILE_S]))
			patterns[n++] = 37;
	}
	if(S_UP(slopes[TILE_S]) && E_UP(slopes[TILE_SE]))
		patterns[n++] = 37;
	if(S_UP(slopes[TILE_SE]) && E_UP(slopes[TILE_E]))
		patterns[n++] = 14;
	if(S_UP(slopes[TILE_NE]))
		patterns[n++] = 20;
	if(S_UP(slopes[TILE_W]))
		patterns[n++] = 21;
	if(slopes[TILE_NW] == SLOPE_SWE_UP)
		patterns[n++] = 21;
	return n;
}

static int
ViewLightSlope_NE_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 1;
	if(FLAT(slopes[TILE_SW]) || S_OR_W_UP(slopes[TILE_SW]))
		patterns[n++] = 10;
	else{
		if(FLAT(slopes[TILE_W]) || W_UP(slopes[TILE_W]))
			patterns[n++] = 26;
		if(FLAT(slopes[TILE_S]) || S_UP(slopes[TILE_S]))
			patterns[n++] = 37;
	}
	if(FLAT(slopes[TILE_NE]) || ANY_SW_UP(slopes[TILE_NE]))
		patterns[n++] = 11;
	else{
		if(FLAT(slopes[TILE_N]))
			patterns[n++] = 36;
		if(FLAT(slopes[TILE_E]))
			patterns[n++] = 14;
	}
	if(E_UP(slopes[TILE_E]) && N_UP(slopes[TILE_NE]))
		patterns[n++] = 14;
	if(E_UP(slopes[TILE_NE]) && N_UP(slopes[TILE_N]))
		patterns[n++] = 36;
	if(N_UP(slopes[TILE_SE]))
		patterns[n++] = 20;
	if(slopes[TILE_NW] == SLOPE_NWE_UP)
		patterns[n++] = 21;
	if(FLAT(slopes[TILE_W]))
		patterns[n++] = 26;
	return n;
}

static int
ViewLightSlope_SWE_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 3;
	if(FLAT(slopes[TILE_N]))
		patterns[n++] = 30;
	if(FLAT(slopes[TILE_SE]) + FLAT(slopes[TILE_SW]) + FLAT(slopes[TILE_S]) > 1)
		patterns[n++] = 31;
	if(FLAT(slopes[TILE_SE]) || ANY_NW_UP(slopes[TILE_SE]))
		patterns[n++] = 9;
	if(FLAT(slopes[TILE_SW]) || ANY_NE_UP(slopes[TILE_SW]))	// bug in AOE, uses SE for two checks
		patterns[n++] = 6;
	if(S_UP(slopes[TILE_S]))
		patterns[n++] = 22;
	if(S_UP(slopes[TILE_SW]))
		patterns[n++] = 23;
	if(S_UP(slopes[TILE_NW]))
		patterns[n++] = 23;
	if(S_UP(slopes[TILE_NE]))
		patterns[n++] = 20;
	if(S_UP(slopes[TILE_SE]))
		patterns[n++] = 20;
	return n;
}

static int
ViewLightSlope_NWE_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 3;
	if(FLAT(slopes[TILE_NE]) + FLAT(slopes[TILE_NW]) + FLAT(slopes[TILE_N]) > 1)
		patterns[n++] = 33;
	if(FLAT(slopes[TILE_S]))
		patterns[n++] = 32;
	if(FLAT(slopes[TILE_NW]) || ANY_SE_UP(slopes[TILE_NW]))
		patterns[n++] = 4;
	if(FLAT(slopes[TILE_NE]) || ANY_SW_UP(slopes[TILE_NE]))
		patterns[n++] = 11;
	if(FLAT(slopes[TILE_E]))
		patterns[n++] = 20;
	if(FLAT(slopes[TILE_W]))
		patterns[n++] = 23;
	if(N_UP(slopes[TILE_SW]))
		patterns[n++] = 23;
	if(N_UP(slopes[TILE_NW]))
		patterns[n++] = 23;
	if(N_UP(slopes[TILE_SE]))
		patterns[n++] = 20;
	if(N_UP(slopes[TILE_NE]))
		patterns[n++] = 20;
	return n;
}

static int
ViewLightSlope_NSE_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 1;
	if(FLAT(slopes[TILE_W]))
		patterns[n++] = 26;
	if(FLAT(slopes[TILE_NE]) + FLAT(slopes[TILE_SE]) + FLAT(slopes[TILE_E]) > 1)
		patterns[n++] = 27;
	if(ANY_W_UP(slopes[TILE_W]))
		patterns[n++] = 26;
	if(FLAT(slopes[TILE_NE]) || ANY_SW_UP(slopes[TILE_NE]))
		patterns[n++] = 11;
	else{
		if(FLAT(slopes[TILE_N]) || N_UP(slopes[TILE_N]))
			patterns[n++] = 36;
		if(N_UP(slopes[TILE_NE]))
			patterns[n++] = 14;
	}
	if(FLAT(slopes[TILE_SE]) || ANY_NW_UP(slopes[TILE_SE]))
		patterns[n++] = 9;
	else{
		if(FLAT(slopes[TILE_S]))
			patterns[n++] = 37;
		if(S_UP(slopes[TILE_SE]))
			patterns[n++] = 14;
	}
	if(slopes[TILE_NW] == SLOPE_NWE_UP)
		patterns[n++] = 21;
	if(slopes[TILE_SW] == SLOPE_SWE_UP)
		patterns[n++] = 26;
	return n;
}

static int
ViewLightSlope_NSW_UP(View *view, int *slopes, int *patterns)
{
	int n;
	n = 0;
	patterns[n++] = 0;
	if(FLAT(slopes[TILE_E]))
		patterns[n++] = 28;
	if(FLAT(slopes[TILE_NW]) + FLAT(slopes[TILE_SW]) + FLAT(slopes[TILE_W]) > 1)
		patterns[n++] = 29;
	if(ANY_E_UP(slopes[TILE_E]))
		patterns[n++] = 28;
	if(FLAT(slopes[TILE_NW]) || ANY_SE_UP(slopes[TILE_NW]))
		patterns[n++] = 4;
	else{
		if(FLAT(slopes[TILE_N]) || N_UP(slopes[TILE_N]))
			patterns[n++] = 34;
		if(N_UP(slopes[TILE_NW]))
			patterns[n++] = 12;
	}
	if(FLAT(slopes[TILE_SW]) || ANY_NE_UP(slopes[TILE_SW]))
		patterns[n++] = 6;
	else{
		if(FLAT(slopes[TILE_S]))
			patterns[n++] = 35;
		if(S_UP(slopes[TILE_SW]))
			patterns[n++] = 12;
	}
	if(slopes[TILE_NE] == SLOPE_NWE_UP)
		patterns[n++] = 22;
	if(slopes[TILE_SE] == SLOPE_SWE_UP)
		patterns[n++] = 28;
	return n;
}
