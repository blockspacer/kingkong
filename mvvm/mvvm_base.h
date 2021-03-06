﻿#ifndef _MVVM_BASE_H
#define _MVVM_BASE_H
#include <string>
#include <map>
#include <memory>
#include <google/protobuf/message.h>

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE() }

#define MVVM_FRAME mvvm::framework
#define BEGIN_NAMESPACE_FRAME BEGIN_NAMESPACE(mvvm) BEGIN_NAMESPACE(framework)
#define END_NAMESPACE_FRAME END_NAMESPACE() END_NAMESPACE()

//UI 用于处理 viewmode 抛上来的属性
#define BEGIN_HANDLE_PROPERTY(property_id) switch (property_id) {
#define HANDLE_PROPERTY(action, func) case action: { return func(new_value);}
#define END_HANDLE_PROPERTY() default: break;}

//UI ViewModel 处理事件
#define BEGIN_HANDLE_EVENT(event_id) switch (event_id) {
#define HANDLE_EVENT(event_id, func) case event_id: { return func(value);}
#define HANDLE_EVENT_RESULT(event_id, func) case event_id: { return func(value, result);}
#define END_HANDLE_EVENT() default: break;}

//ViewModel 订阅Model 事件
#define BEGIN_SUBSCRIE_EVENT(model_type) if (auto model = MVVM_FRAME::Model::ModelOf(model_type)) { \
auto tmp_model_type = model_type;
#define SUBSCRIE_EVENT(event_id, func)  {\
int64_t id = model->SubscribeEvent(event_id, std::bind(func, this, std::placeholders::_1, std::placeholders::_2));\
subscribe_ids_[id] = tmp_model_type; }
#define END_SUBSCRIE_EVENT() }

#endif