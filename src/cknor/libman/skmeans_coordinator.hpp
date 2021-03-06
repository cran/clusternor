/*
 * Copyright 2016 neurodata (http://neurodata.io/)
 * Written by Disa Mhembere (disa@jhu.edu)
 *
 * This file is part of knor
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY CURRENT_KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __KNOR_SKMEANS_COORDINATOR_HPP__
#define __KNOR_SKMEANS_COORDINATOR_HPP__

#include "coordinator.hpp"
#include "util.hpp"

namespace knor {

namespace base {
    class clusters;
}

class skmeans_coordinator : public coordinator {
    private:
        void bounds_reduction();
        std::vector<double> g_feature_max;
        std::vector<double> g_feature_min;

    protected:
        // Metadata
        // max index stored within each threads partition
        std::shared_ptr<base::clusters> cltrs;

        skmeans_coordinator(const std::string fn, const size_t nrow,
                const size_t ncol, const unsigned k, const unsigned max_iters,
                const unsigned nnodes, const unsigned nthreads,
                const double* centers, const base::init_t it,
                const double tolerance, const base::dist_t dt);

    public:
        static coordinator::ptr create(const std::string fn,
                const size_t nrow,
                const size_t ncol, const unsigned k, const unsigned max_iters,
                const unsigned nnodes, const unsigned nthreads,
                const double* centers=NULL, const std::string init="kmeanspp",
                const double tolerance=-1, const std::string dist_type="cos") {

            base::init_t _init_t = base::get_init_type(init);
            base::dist_t _dist_t = base::get_dist_type(dist_type);

            return coordinator::ptr(
                    new skmeans_coordinator(fn, nrow, ncol, k, max_iters,
                    nnodes, nthreads, centers, _init_t, tolerance, _dist_t));
        }

        std::shared_ptr<base::clusters> get_gcltrs() {
            return cltrs;
        }

        // Pass file handle to threads to read & numa alloc
        virtual base::cluster_t run(double* allocd_data,
                const bool numa_opt) override;
        void update_clusters();
        void kmeanspp_init() override;
        void random_partition_init() override;
        void forgy_init() override;
        virtual void preprocess_data();
        virtual void build_thread_state() override;
};
}
#endif
