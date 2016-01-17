class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :timeline_token
      t.string :anilist_token

      t.timestamps null: false
    end
  end
end
