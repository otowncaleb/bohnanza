#include <algorithm>
#include <iterator>
#include "TradeArea.h"
#include "GameExceptions.h"

TradeArea::TradeArea(istream& in, CardFactory *cf) {
	istream_iterator<char> in_iter(in), end;
	transform(in_iter, end, back_inserter(cards), [&cf](char c) { return cf->loadCard(c); });
}

TradeArea& TradeArea::operator+=(Card *c) {
	cards.push_back(c);
	return *this;
}

bool TradeArea::legal(Card *c) const {
	auto pos = find_if(cards.cbegin(), cards.cend(), [c](Card* cardInTradeArea) { return Card::areSameType(c, cardInTradeArea); });
	return pos != cards.cend();
}

Card* TradeArea::trade(string gemName) {
	auto pos = find_if(cards.cbegin(), cards.cend(), [=](Card* cardInTradeArea) { return gemName == cardInTradeArea->getName(); });
	if (pos == cards.cend()) {
		throw game_logic_exception("No cards of the given gemstone are in the Trade Area!");
	}
	Card *c = *pos;
	cards.erase(pos);
	return c;
}

int TradeArea::numCards() {
	return (int)cards.size();
}

std::ostream & operator<<(ostream & out, const TradeArea &tradeArea) {
	ostream_iterator<Card*> it(out, " ");
	copy(tradeArea.cards.cbegin(), tradeArea.cards.cend(), it);
	return out;
}
