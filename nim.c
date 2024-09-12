#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Reads a Nim game state from standard input.  The number of piles
 * should be the first value to read and will be stored in the
 * reference argument.  The size of each pile should follow and will
 * be stored in an array returned as the return value.  It is the caller's
 * responsibility to free that array.  If there are missing values then
 * they are filled with zeros.  If the array can't be allocated then
 * the return value is null and the number of piles is set to 0.
 *
 * @param num_piles a pointer to an unsigned int, non-NULL
 * @return an array whose size is the value stored in num_piles, or NULL
 */
unsigned int *read_game(size_t *num_piles);


/**
 * Finds a winning move for the given state of Nim.  If there is a winning
 * move, then the index of
 * a pile to take stones from and the number of stones to take are
 * stored in the reference arguments and the return value is true.  If
 * there is no winning move then the return value is false.
 *
 * @param num_piles a nonnegative integer
 * @param pile_size an array of that many unsigned integers
 * @param move_pile a pointer to an index, non-NULL
 * @param move_count a pointer to an unsigned integer, non-NULL
 * @return true if and only if there is a winning move
 */
bool find_winning_move(size_t num_piles, unsigned int pile_size[], size_t *move_pile, unsigned int *move_count);


/**
 * Returns the largest value in the given array.
 *
 * @param num_piles a positive integer
 * @param pile_size an array of that many unsigned integers
 * @return an index into that array containing the largest value
 */
size_t find_max(size_t size, unsigned int arr[]);


/**
 * Determines the place value of the most significant 1 in the binary
 * representation of the given unsigned integer.
 *
 * @param z a positive integer
 * @return the power of two so that z is between that power of two
 * (inclusive) and the next highest power of two
 */
unsigned int find_msb(unsigned int z);


/**
 * Determines if the given bit in the given unsigned integer is set.
 *
 * @param num an unsigned integer
 * @param b a power of two
 * @return true if and only if the binary representation of num has a 1
 * in the place with place value b
 */
bool is_bit_set(unsigned int num, unsigned int b);


int main()
{
  size_t num_piles;
  unsigned int *pile_size = read_game(&num_piles);

  if (num_piles == 0)
    {
      fprintf(stdout, "GAME OVER\n");
      return 0;
    }

  size_t take_row;
  unsigned int take_count;
  if (find_winning_move(num_piles, pile_size, &take_row, &take_count))
    {
      fprintf(stdout, "WIN: take %u from pile %zu\n", take_count, take_row);
    }
  else
    {
      size_t largest_pile = find_max(num_piles, pile_size);
      if (pile_size[largest_pile] > 0)
	{
	  fprintf(stdout, "LOSE: take 1 from pile %zu\n", largest_pile);
	}
      else
	{
	  fprintf(stdout, "GAME OVER\n");
	}
    }   
  
  free(pile_size);

  return 0;
}


unsigned int find_msb(unsigned int z)
{
  if (z == 0)
    {
      return 0;
    }
  else
    {
      unsigned int curr_bit = 1;
      unsigned int msb;
      while (curr_bit <= z && curr_bit != 0)
	{
	  if ((curr_bit & z) != 0)
	    {
	      msb = curr_bit;
	    }
	  curr_bit = curr_bit << 1;
	}

      return msb;
    }
}


bool is_bit_set(unsigned int num, unsigned int b)
{
  return ((num & b) != 0);
}


size_t find_max(size_t size, unsigned int arr[])
{
  size_t index_max = 0;
  unsigned int max = arr[0];
  for (size_t i = 1; i < size; i++)
    {
      if (arr[i] > max)
	{
	  max = arr[i];
	  index_max = i;
	}
    }
  return index_max;
}


unsigned int *read_game(size_t *num_piles)
{
  // read number of piles
  fscanf(stdin, "%zu", num_piles);

  unsigned int pile_size[*num_piles]; // BROKEN -- doesn't survive the return
  if (pile_size == NULL)
    {
      // couldn't create the array
      *num_piles = 0;
      return NULL;
    }

  // read each pile size
  size_t i = 0;
  while (i < *num_piles && fscanf(stdin, "%u", &pile_size[i]) == 1)
    {
      i++;
    }
  
  // fill in any missing values with 0
  while (i < *num_piles)
    {
      pile_size[i] = 0;
      i++;
    }
  
  return pile_size;  // BROKEN -- can't return ptr to stack-allocated array
}


bool find_winning_move(size_t num_piles, unsigned int pile_size[], size_t *move_pile, unsigned int *move_count)
{
  // compute bitwise exclusive or of all pile sizes
  unsigned int xor = 0;
  for (size_t i = 0; i < num_piles; i++)
    {
      xor = xor ^ pile_size[i];
    }

  // no winning move if and only if xor is 0
  if (xor == 0)
    {
      return false;
    }

  // find most significant (leftmost) 1 in binary representation of xor
  unsigned int msb = find_msb(xor);

  // find a pile size that has that bit set
  *move_pile = 0;
  while (!is_bit_set(pile_size[*move_pile], msb))
    {
      (*move_pile)++;
    }

  // compute xor to determine number of stones to leave
  *move_count = pile_size[*move_pile] - (xor ^ pile_size[*move_pile]);
  
  return true;
}
