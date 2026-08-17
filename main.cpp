#include <iostream>
#include <cstdint>
#include <queue> 
#include <map>


enum class Side : uint8_t {
    BUY = 0,
    SELL = 1
};

struct Order {
    Side side;
    uint32_t quantity;
    uint64_t price;
    uint64_t timestamp;
    uint64_t orderId;
};

struct Level{ 
    uint64_t price;
    std::queue<Order> order;
};

struct OrderBook { 
    std::map<uint64_t, Level> bids;
    std::map<uint64_t, Level> asks;

    void addOrder(Order newOrder){
        if (newOrder.side == Side::BUY){
            bids[newOrder.price].price = newOrder.price;

            bids[newOrder.price].order.push(newOrder);

        }else{
            asks[newOrder.price].price = newOrder.price;

            asks[newOrder.price].order.push(newOrder);
        }
    }
};


int main(){
    Order order;
    order.side = Side::BUY;
    order.orderId = 123456789;
    order.quantity = 100;
    order.price = 5000;

    OrderBook Orderbook;

    Orderbook.addOrder(order);

    std::cout << "Order Details:" << std::endl;
    std::cout << "Side: " << (order.side == Side::BUY ? "BUY" : "SELL") << std::endl;
    std::cout << "Order ID: " << order.orderId << std::endl;
    std::cout << "Quantity: " << order.quantity << std::endl;
    std::cout << "Price: " << order.price << std::endl;

    std::cout << "Number of $50.00 bids: " << Orderbook.bids[5000].order.size() << std::endl;

    return 0;
}