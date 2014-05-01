#ifndef PRINT_DBG
#ifdef DEBUG
#define D(x) x;
#define D_VAR(x) (std::cerr << #x << ": " << (x) << std::endl);
#else
#define D(X)
#define D_VAR(x)
#endif
#endif
