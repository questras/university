#ifndef SRC_MULTITHREADEDPAGERANKCOMPUTER_HPP_
#define SRC_MULTITHREADEDPAGERANKCOMPUTER_HPP_

#include <atomic>
#include <mutex>
#include <thread>

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "immutable/network.hpp"
#include "immutable/pageIdAndRank.hpp"
#include "immutable/pageRankComputer.hpp"

using std::ref;
using std::thread;
using std::vector;


void generateIdParallel(std::atomic<int> &pageCounter, vector<Page> const &pages, int pagesCount,
                        const IdGenerator &generator) {
    int myPageIndex;
    while (true) {
        myPageIndex = pageCounter++;

        if (myPageIndex >= pagesCount) {
            return;
        }

        pages[myPageIndex].generateId(generator);
    }
}

void calculatePageRankParallel(
        std::atomic<int> &pageCounter,
        std::unordered_map<PageId, PageRank, PageIdHash> &pageHashMap,
        std::unordered_map<PageId, PageRank, PageIdHash> &previousPageHashMap,
        std::mutex &mutex,
        int networkSize,
        double alpha,
        double &previousDangleSum,
        double &newDangleSum,
        double danglingWeight,
        double &difference,
        std::unordered_map<PageId, std::vector<PageId>, PageIdHash> &edges,
        std::unordered_map<PageId, uint32_t, PageIdHash> &numLinks,
        std::vector<PageId> &ids,
        std::unordered_set<PageId, PageIdHash> &danglingNodes) {
    int myPageIndex;
    double updatedValue, diff, dangleUpdate;

    while (true) {
        myPageIndex = pageCounter++;
        if (myPageIndex >= networkSize) {
            return;
        }

        PageId pageId = ids[myPageIndex];
        // Adding sum of dangling nodes
        updatedValue = previousDangleSum * danglingWeight + (1.0 - alpha) / networkSize;

        // Update page rank regarding all pages that link to current page.
        if (edges.count(pageId) > 0) {
            for (auto link : edges[pageId]) {
                updatedValue += alpha * previousPageHashMap[link] / numLinks[link];
            }
        }
        pageHashMap[pageId] = updatedValue;
        diff = std::abs(previousPageHashMap[pageId] - updatedValue);

        if (danglingNodes.count(pageId) > 0) {
            dangleUpdate = updatedValue;
        }
        else {
            dangleUpdate = 0;
        }

        std::lock_guard<std::mutex> lock(mutex);
        difference += diff;
        newDangleSum += dangleUpdate;
    }
}

class MultiThreadedPageRankComputer : public PageRankComputer {
public:
    MultiThreadedPageRankComputer(uint32_t numThreadsArg)
            : numThreads(numThreadsArg) {};

    std::vector<PageIdAndRank>
    computeForNetwork(Network const &network, double alpha, uint32_t iterations, double tolerance) const {
        vector<thread> threads;
        vector<PageId> ids;
        vector<Page> const &pages = network.getPages();
        std::unordered_map<PageId, PageRank, PageIdHash> pageHashMap;
        std::unordered_map<PageId, uint32_t, PageIdHash> numLinks;
        std::unordered_set<PageId, PageIdHash> danglingNodes;
        std::unordered_map<PageId, std::vector<PageId>, PageIdHash> edges;
        auto networkSize = network.getSize();

        // ID generating.
        std::atomic<int> pageCounter;
        pageCounter = 0;
        auto const &generator = network.getGenerator();

        for (uint32_t i = 0; i < this->numThreads; i++) {
            threads.push_back(thread(generateIdParallel, ref(pageCounter), ref(pages), networkSize, ref(generator)));
        }

        for (uint32_t i = 0; i < this->numThreads; i++) {
            threads[i].join();
        }

        for (auto const &page : network.getPages()) {
            // initial pageHashMap value (1/n)
            pageHashMap[page.getId()] = 1.0 / network.getSize();
            ids.push_back(page.getId());

            // Setting number of links for each page.
            numLinks[page.getId()] = page.getLinks().size();

            // Setting set with dangling nodes
            if (page.getLinks().size() == 0) {
                danglingNodes.insert(page.getId());
            }

            // For each page setting up pages that link to it.
            for (auto link : page.getLinks()) {
                edges[link].push_back(page.getId());
            }
        }

        double previousDangleSum = 0, newDangleSum = 0;
        for (auto danglingNode : danglingNodes) {
            previousDangleSum += pageHashMap[danglingNode];
        }
        newDangleSum = previousDangleSum;

        // Perform page rank iterations.
        for (uint32_t i = 0; i < iterations; ++i) {
            std::unordered_map<PageId, PageRank, PageIdHash> previousPageHashMap = pageHashMap;

            previousDangleSum = newDangleSum;
            previousDangleSum = previousDangleSum * alpha;
            newDangleSum = 0;
            double danglingWeight = 1.0 / network.getSize();

            // Count new page rank for each page.
            double difference = 0;
            std::mutex mutex;
            threads.clear();
            pageCounter = 0;

            for (uint32_t k = 0; k < this->numThreads; k++) {
                threads.push_back(thread(
                        calculatePageRankParallel,
                        ref(pageCounter),
                        ref(pageHashMap),
                        ref(previousPageHashMap),
                        ref(mutex),
                        networkSize,
                        alpha,
                        ref(previousDangleSum),
                        ref(newDangleSum),
                        danglingWeight,
                        ref(difference),
                        ref(edges),
                        ref(numLinks),
                        ref(ids),
                        ref(danglingNodes)));
            }

            for (uint32_t k = 0; k < this->numThreads; k++) {
                threads[k].join();
            }


            // Check if we need to finish iterations.
            if (difference < tolerance) {
                std::vector<PageIdAndRank> result;
                for (auto iter : pageHashMap) {
                    result.push_back(PageIdAndRank(iter.first, iter.second));
                }

                return result;
            }
        }

        ASSERT(false, "Not able to find result in iterations=" << iterations);
    }

    std::string getName() const {
        return "MultiThreadedPageRankComputer[" + std::to_string(this->numThreads) + "]";
    }

private:
    uint32_t numThreads;
};

#endif /* SRC_MULTITHREADEDPAGERANKCOMPUTER_HPP_ */
