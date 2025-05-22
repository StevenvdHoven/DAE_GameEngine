#pragma once
#include "Command.h"

namespace Engine
{
	class OpenGraphEditor final : public Command
	{
	public:
		OpenGraphEditor() = default;
		~OpenGraphEditor() override = default;

		void Execute() override;
	};

	class AddGraphNodeCommand final : public Command
	{
	public:
		AddGraphNodeCommand() = default;
		~AddGraphNodeCommand() override = default;

		void Execute() override;
	};

	class DeleteGraphNodeCommand final : public Command
	{
	public:
		DeleteGraphNodeCommand() = default;
		~DeleteGraphNodeCommand() override = default;

		void Execute() override;
	};

	class SaveGraphCommand final : public Command
	{
	public:
		SaveGraphCommand() = default;
		~SaveGraphCommand() override = default;

		void Execute() override;
	};

}



