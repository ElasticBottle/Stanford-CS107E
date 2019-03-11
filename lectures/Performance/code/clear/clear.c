#include "printf.h"
#include "fb.h"
#include "assert.h"
#include "system.h"
#include "timer.h"

#define _WIDTH 640
#define _HEIGHT 512
#define _DEPTH 4

struct pixel {
    unsigned char b, g, r, alpha;
};

typedef struct pixel pixel_t;

static pixel_t *fb;

#define TIME(cmd, val, iter) do {                       \
        printf("----------\n");                         \
        printf("running: " #cmd "\n");                  \
        int i;                                          \
        unsigned int agg = 0;                           \
        for (i = 0; i < iter; i++) {                    \
            unsigned start = timer_get_ticks();          \
            cmd(0x00);                                  \
            cmd(val);                                   \
            unsigned total = timer_get_ticks() - start; \
            agg+= total;                                \
        }                                               \
        printf("avg %d ticks\n", agg/iter);            \
    } while (0)                                         \


/**************** Simple unoptimized write_screen *******************/
inline static void draw_pixel(unsigned char v, int x, int y) {
    pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)[fb_get_width()])fb;
    pixel_t *p = &fb_s[y][x];
    p->r = v;
    p->g = v;
    p->b = v;
    p->alpha = 0xff;
}

void write_screen(unsigned char v) {
    for (unsigned i = 0; i < fb_get_height(); i++) {
        for (unsigned j = 0; j < fb_get_width(); j++) {
            draw_pixel(v, j, i);
        }
    }
}

/**************** remove func overhead write_screen *******************/
void write_screen_remove_func(unsigned char v) {
    for (unsigned i = 0; i < fb_get_height(); i++) {
        for (unsigned j = 0; j < fb_get_width(); j++) {
            pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)[fb_get_width()])fb;
            pixel_t *p = &fb_s[i][j];
            p->r = v;
            p->g = v;
            p->b = v;
            p->alpha = 0xff;
        }
    }
}


/**************** multi byte write_screen *******************/
void write_screen_bitpattern(unsigned char v) {
    for (unsigned i = 0; i < fb_get_height(); i++) {
        for (unsigned j = 0; j < fb_get_width(); j++) {
            pixel_t (*fb_s)[fb_get_width()] = (pixel_t (*)
                    [fb_get_width()])fb;
            pixel_t *p = &fb_s[i][j];
            *(unsigned *)p =  (0xff << 24) | (v << 16) | (v << 8) | v;
        }
    }
}

/**************** unsigned write_screen *******************/
// store all of p as an unsigned integer
void write_screen_unsigned(unsigned char v) {
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;
    for (unsigned i = 0; i < fb_get_height(); i++) {
        for (unsigned j = 0; j < fb_get_width(); j++) {
            unsigned (*fb_s)[fb_get_width()] = (unsigned (*)
                    [fb_get_width()])fb;
            *(unsigned *)fb_s[i][j] = *(unsigned *)&p;
        }
    }
}


/**************** cache local var write_screen *******************/
void write_screen_hoist(unsigned char v) {
    int n = fb_get_height() * fb_get_width();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;    
    for (unsigned i = 0; i < n; i++) {
        ((unsigned *)fb)[i] = *(unsigned *) &p;
    }
}

/**************** unsigned long write_screen *******************/
void write_screen_ull(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    unsigned long long u = *(unsigned *)&p;
    unsigned long long uu = (u << 32) | u;
    // assuming n is even 
    // assuming fb is 8 b ytes align 
    for (unsigned i = 0; i < n / 2; i++) {
        ((unsigned long long *)fb)[i] = uu;
    }
}

/**************** unsigned long write_screen *******************/
void write_screen_unroll(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;

    unsigned long long u = *(unsigned *)&p;
    unsigned long long uu = (u << 32) | u;
    // assume n is multiple of 16
    // fb is 16 byte aligned
    for (unsigned i = 0; i < n / 2; i+=8) {
        unsigned long long *fb_ll = (unsigned long long *)&fb[i];
        fb_ll[0] = uu;
        fb_ll[1] = uu;
        fb_ll[2] = uu;
        fb_ll[3] = uu;
        fb_ll[4] = uu;
        fb_ll[5] = uu;
        fb_ll[6] = uu;
        fb_ll[7] = uu;
    }
}

void assign8(void *dst, unsigned u, unsigned n);

void write_screen_asm(unsigned char v) {
    unsigned n = fb_get_width() * fb_get_height();
    pixel_t p;
    p.r = p.g = p.b = v;
    p.alpha = 0xff;
    assign8((void *) fb, *(unsigned *) &p, n / 8);
}

void main() {
    //printf_init();
    timer_init();

    fb_init(_WIDTH, _HEIGHT, _DEPTH, 0);
    fb = (struct pixel *) fb_get_draw_buffer();
    system_enable_dcache();
    system_enable_icache();
    system_enable_branch_prediction();

    TIME(write_screen, 0xff, 3); 
    //TIME(write_screen_remove_func, 0xff, 3); 
    //TIME(write_screen_bitpattern, 0xff, 3);
    //TIME(write_screen_unsigned, 0xff, 3); 
    //TIME(write_screen_hoist, 0xff, 3); 
    //TIME(write_screen_ull, 0xff, 3); 
    //TIME(write_screen_unroll, 0xff, 3);
    TIME(write_screen_asm, 0xff, 100);

    printf("%c", 4); // Send EOT to signal done.
}
