/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelineFactory.cpp
@Time: 2021/6/23 10:25 下午
@Desc: 
***************************/

#include "GPipelineFactory.h"

CGRAPH_NAMESPACE_BEGIN

GPipelinePtrList GPipelineFactory::s_pipeline_list_;
std::mutex GPipelineFactory::s_lock_;

GPipelinePtr GPipelineFactory::create() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(s_lock_);

    if (s_pipeline_list_.empty()) {
        status = UThreadPoolSingleton::get()->init();
        if (STATUS_OK != status) {
            return nullptr;
        }
    }

    GPipelinePtr pipeline = CGRAPH_SAFE_MALLOC_COBJECT(GPipeline);
    s_pipeline_list_.emplace_back(pipeline);
    return pipeline;
}


void GPipelineFactory::destroy(GPipelinePtr pipeline) {
    if (nullptr == pipeline) {
        return;
    }

    CGRAPH_LOCK_GUARD lock(s_lock_);
    s_pipeline_list_.remove(pipeline);
    CGRAPH_DELETE_PTR(pipeline)

    if (s_pipeline_list_.empty()) {
        UThreadPoolSingleton::get()->deinit();
    }
}


void GPipelineFactory::clear() {
    CGRAPH_LOCK_GUARD lock(s_lock_);

    for (GPipelinePtr pipeline : GPipelineFactory::s_pipeline_list_) {
        CGRAPH_DELETE_PTR(pipeline)
    }

    UThreadPoolSingleton::get()->deinit();
    s_pipeline_list_.clear();
}


CSTATUS GPipelineFactory::run() {
    CGRAPH_NO_SUPPORT
}

CGRAPH_NAMESPACE_END
