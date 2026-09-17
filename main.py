from google import genai

client = genai.Client(api_key="YOUR_API_KEY")

while True:
    user = input("تو: ")

    if user.lower() in ["exit", "quit", "خروج"]:
        break

    response = client.interactions.create(
        model="gemini-3.8-flash",
        input=user
    )

    print("AI:", response.output_text)
