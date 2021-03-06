#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

void stripWhiteSpace(char *str)
{
	char *p = strchr(str, '\n');
	if (NULL == p)
	{
		p = strchr(str, '\t');
	}
	*p = '\0';
}

deck_t *hand_from_string(const char *str, future_cards_t *fc)
{
	size_t len = strlen(str);
	if ('\0' == *str || len < 2)
	{
		return NULL;
	}
	size_t i;
	size_t j;
	deck_t *hand = create_deck();
	for (i = 0; i < len - 1; ++i)
	{
		if (isspace(str[i]))
		{
			continue;
		}
		for (j = i + 1; j < len && (j != '\n' || '\0' != j); ++j)
		{
			if ('?' == str[i])
			{
				size_t index = atoi(&str[j]);
				card_t *c = add_empty_card(hand);
				add_future_card(fc, index, c);
			}
			else if ((isspace(str[j + 1]) || str[j + 1] == '\0') && isalnum(str[i]) && isalpha(str[j]))
			{
				card_t card = card_from_letters(str[i], str[j]);
				add_card_to(hand, card);
			}
			i = j;
		}
	}
	return hand;
}
deck_t **read_input(FILE *f, size_t *n_hands, future_cards_t *fc)
{
	size_t i = 0;
	char *line = NULL;
	size_t size = 0;
	size_t sz = 0;
	deck_t **hands = NULL;
	while ((size = getline(&line, &sz, f)) != EOF)
	{
		stripWhiteSpace(line);
		if (0 == strlen(line))
		{
			continue;
		}
		hands = realloc(hands, (i + 1) * sizeof(*hands));
		assert(hands && "malloc failed for hands");
		hands[i] = hand_from_string(line, fc);
		assert(5 <= hands[i]->n_cards && "hand has less than 5 cards");
		free(line);
		line = NULL;
		++i;
	}
	*n_hands = i;
	free(line);
	return hands;
}
