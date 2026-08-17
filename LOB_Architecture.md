# High-Frequency Limit Order Book (LOB) Architecture

## The Rules of Engagement
1. **The Socratic Code Review:** The AI acts as a senior reviewer. No final copy-paste code.
2. **Bottom-Up Development:** Microscopic (Bytes -> Structs -> Algorithms).
3. **Progressive Overload:** V1 uses standard libraries (`std::queue`, `std::map`). V2 rips them out for custom memory pools.
4. **Just-In-Time Learning:** Research syntax only when an architectural roadblock requires it.

## Architectural Decision Record (ADR)
*Tracking the "Why" behind memory and data type decisions.*
* **Order ID:** `uint64_t` (8 bytes) - To handle 18 quintillion unique orders without overflowing.
* **Price:** `uint64_t` (8 bytes) - Stored as Integer Ticks. Floating-point (`float`/`double`) causes rounding errors in finance.
* **Quantity:** `uint32_t` (4 bytes) - Orders cannot be negative; saves 4 bytes compared to a 64-bit integer.
* **Side:** `enum class : uint8_t` (1 byte) - Safer and more readable than a raw boolean flag, but takes the exact same memory footprint.
* **Timestamp:** `uint64_t` (8 bytes) - To hold exact nanoseconds since the Unix epoch.

---

## Phase 1: Core Primitives (Completed)
```cpp
#include <cstdint>

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

## Phase 2: The Resting Book & Price Levels (Completed)

### The Hierarchy
1. **Level:** A single price point containing a FIFO queue (`std::queue`) of `Order` structs. Time-priority is naturally enforced by the queue.
2. **OrderBook:** Holds two `std::map` structures (`bids` and `asks`). 

### Why std::map?
`std::map` is implemented as a Red-Black tree. It automatically sorts its keys (Prices). This guarantees we can instantly locate the Best Bid and Best Ask without wasting CPU cycles on manual sorting algorithms. 

```cpp
#include <queue>
#include <map>

struct Level {
    uint64_t price;
    std::queue<Order> order;
};

struct OrderBook {
    std::map<uint64_t, Level> bids;
    std::map<uint64_t, Level> asks;

    void addOrder(Order newOrder) {
        if (newOrder.side == Side::BUY) {
            bids[newOrder.price].price = newOrder.price;
            bids[newOrder.price].order.push(newOrder);
        } else {
            asks[newOrder.price].price = newOrder.price;
            asks[newOrder.price].order.push(newOrder);
        }
    }
};