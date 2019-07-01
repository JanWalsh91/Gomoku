class AAction {

public:

	enum Type {
		DECREMENT_CAPTURE,
		INCREMENT_CAPTURE,
		SET_END_STATE,
		UPDATE_BOARD
	};

	AAction(Type type): type(type) {}

	virtual ~AAction() {}

	Type type;
};