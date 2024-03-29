#include <algorithm>
#include <iterator>
#include <random>
#include <string>
#include "Card.h"
#include "CardFactory.h"
#include "GameExceptions.h"

using namespace std;

/* Add card pointers to the deck and multimap */
template <class C> void CardFactory::addCards(int n) {
	for (int i = 0; i < n; i++) {
		Card* c = new C;
		deck.push_back(c);
		unloadedCards.insert({ c->getName()[0], c });
	}
}

CardFactory::CardFactory() : deck(Deck()), unloadedCards() {
	deck.reserve(numCardsInDeck);
	addCards<Quartz>(20);
	addCards<Hematite>(18);
	addCards<Obsidian>(16);
	addCards<Malachite>(14);
	addCards<Turquoise>(12);
	addCards<Ruby>(10);
	addCards<Amethyst>(8);
	addCards<Emerald>(6);
}

CardFactory* CardFactory::instance = nullptr;

CardFactory* CardFactory::getFactory() {
		if (instance == nullptr) {
		instance = new CardFactory;
	}
	return instance;
}

Deck CardFactory::getDeck() {
	std::shuffle(deck.begin(), deck.end(), default_random_engine());
	return deck;
}

/* Used for constructing games from a file. 
 * Given a character, this method returns a pointer to a Card that has not yet been added to Card container
 * The multimap allows us to quickly locate such a Card.
 * If no Cards are left in the multimap for a given character, then the save file is corrupt. */
Card* CardFactory::loadCard(char c) {
	auto iterators = unloadedCards.equal_range(c); //iterators for the Cards corresponding to character "c"
	if (iterators.first == iterators.second) { //there are no more Cards of this type left to allocate
		throw corrupt_game_file_exception();
	}
	Card *card = iterators.first->second; //get a card of this type
	unloadedCards.erase(iterators.first); //remove the card from the set
	return card;
}

void CardFactory::resetUnloadedCards() {
	unloadedCards.clear();
	for each (Card* c in deck) {
		unloadedCards.insert({ c->getName()[0], c });
	}
}

