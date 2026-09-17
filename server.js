from openai import OpenAI

client = OpenAI()

session = client.beta.agents.sessions.create(
    agent={
        "model": "gpt-6-astra",
        "instructions": "Use the OpenAI documentation MCP and web search to answer technical questions accurately. Delegate independent research tasks to subagents when useful.",
        "tools": [
            {"type": "programmatic_tool_calling"},
            {
                "type": "mcp",
                "server_label": "openai_docs",
                "transport": {
                    "type": "http",
                    "server_url": "https://developers.openai.com/mcp",
                },
            },
            {"type": "web_search"},
        ],
        "multi_agent": {"enabled": True, "max_concurrent_subagents": 4},
    },
    environment={
        "type": "self_hosted",
        "workspace_directory": "/workspace",
        "capability_directories": ["/workspace/capabilities/skills"],
    },
    input=[
        {
            "role": "user",
            "content": [
                {
                    "type": "input_text",
                    "text": "Research how to connect an MCP server to an OpenAI agent, check for recent updates, and summarize the recommended setup.",
                }
            ],
        }
    ],
)
print(session.id)
