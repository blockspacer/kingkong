#include "model.h"
#include <boost/assert.hpp>
BEGIN_NAMESPACE_FRAME

int64_t Model::s_suscribe_id_ = 0;
std::map<ModelType, Model::ModelBuilder> Model::model_builder_;
std::map<ModelType, Model*> Model::models_;


int64_t Model::SubscribeEvent(int32_t event, SuscribeEventDelegate delegate) {
	auto delegate_iter = event_delegate_.find(event);
	if (delegate_iter == event_delegate_.end()) {
		delegate_iter = event_delegate_.insert(std::make_pair(event, std::vector<SuscribeEventDelegateInfo>())).first;
	}

	SuscribeEventDelegateInfo info(delegate, ++s_suscribe_id_);
	delegate_iter->second.push_back(std::move(info));

	return s_suscribe_id_;
}
//需要遍历整个事件列表
void Model::UnSubscribeEvent(int64_t id) {
  for (auto begin = event_delegate_.begin(); begin != event_delegate_.end(); begin++) {
		auto& delegates = begin->second;
		for (auto delegate = delegates.begin(); delegate != delegates.end(); delegate++) {
			if (id == delegate->id) {
				delegates.erase(delegate);
				return;
			}
		}
  }
}

void Model::FireEvent(int event, const boost::any& value) {
	auto delegate_iter = event_delegate_.find(event);
	if (delegate_iter == event_delegate_.end()) {
		return;
	}
	for (auto& item : delegate_iter->second) {
		item.delegate(event, value);
	}
}

void Model::RegisterModel(ModelType type, ModelBuilder builder) {
	model_builder_[type] = std::move(builder);
}

Model* Model::ModelOf(ModelType model_type) {
	//先从缓存中找
	auto it = models_.find(model_type);
	if (it != models_.end()) {
		return it->second;
	}
	//没有找到就创建一个
	auto build_iter = model_builder_.find(model_type);
	if (build_iter == model_builder_.end())	{
		return nullptr;
	}
	auto model = build_iter->second();
	models_[model_type] = model;
	return model;
}

END_NAMESPACE_FRAME


