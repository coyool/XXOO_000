/**
 * Ê±ÖÓÐ¾Æ¬RX8025TÇý¶¯
 * Ð»ÎÄ¸Õ
 * 2011.09.11
 */
#ifndef __RX8025_H
#define __RX8025_H

void rx8025_init(void);
void rx8025_write_ymdwhms(const unsigned char *str);
void rx8025_read_ymdwhms(unsigned char *str);


#define  read_ymdwhms rx8025_read_ymdwhms
#define  write_ymdwhms rx8025_write_ymdwhms

#endif

